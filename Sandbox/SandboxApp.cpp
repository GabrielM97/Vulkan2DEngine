#include "SandboxApp.h"

#include <GLFW/glfw3.h>

#include "Component/Gameplay/PlayerMovementComponent.h"
#include "Editor/EditorLayer.h"

void SandboxApp::OnInit()
{
    CreateDefaultScene();
}

void SandboxApp::CreateDefaultScene()
{
    m_Player = m_Scene.Place<Player>();
    //m_Player.SetPosition(glm::vec2{300.0f, 100.0f});
    
    Entity weapon = m_Scene.CreateEntity("Weapon", m_Player.GetID());
    weapon.SetPosition({300.0f, 0.0f});
    weapon.SetSpriteSize({16.0f, 16.0f});
    weapon.SetSpriteTexturePath("Assets/Textures/texture.jpg");
    weapon.SetSpriteLayer(1);

    Entity weapon2 = m_Scene.CreateEntity("Weapon", m_Player.GetID());
    weapon2.SetLocalPosition({-25.0f, 0.0f});
    weapon2.SetSpriteSize({16.0f, 16.0f});
    weapon2.SetSpriteTexturePath("Assets/Textures/texture.jpg");
    weapon2.SetSpriteLayer(1);
    
    Entity map = m_Scene.CreateEntity("Map");
    map.EnsureTileMap();
    map.ResizeTileMap(16, 16);
    map.SetTileSize({32.0f, 32.0f});
    map.SetTileMapGrid(8, 8);
    map.SetTileMapTexturePath("Assets/Textures/texture.jpg");
}

void SandboxApp::RefreshRuntimeHandles()
{
    if (m_Scene.IsValidGameObject(m_Player.GetID()))
    {
        m_Player = Player(m_Scene.GetEntity(m_Player.GetID()));
    }
    else
    {
        m_Player = {};
    }
}

bool SandboxApp::IsEditorPlaying() const
{
    return m_EditorMode == EditorMode::Playing;
}

void SandboxApp::OnEditorPlay()
{
    EnterPlayMode();
}

void SandboxApp::OnEditorStop()
{
    ExitPlayMode();
}

void SandboxApp::EnterPlayMode()
{
    if (m_EditorMode == EditorMode::Playing)
        return;

    if (!m_Scene.SaveToFile(m_PlayModeSnapshotPath))
        return;

    m_EditorMode = EditorMode::Playing;
    m_Scene.BeginPlay();
}

void SandboxApp::ExitPlayMode()
{
    if (m_EditorMode != EditorMode::Playing)
        return;

    m_Scene.EndPlay();

    if (m_Scene.LoadFromFile(m_PlayModeSnapshotPath))
        RefreshRuntimeHandles();

    m_EditorMode = EditorMode::Editing;
}

bool SandboxApp::TryGetHoveredTile(Entity entity, glm::ivec2& outTile) const
{
    if (!entity.IsValid() || !entity.HasTileMap())
        return false;

    const SceneViewportState& viewportState = GetSceneViewportState();
    const glm::vec2 mouseScreen = GetInputState().GetMouseScreenPosition();
    const glm::vec2 worldPosition = ScreenToWorld(mouseScreen, viewportState);
    const Transform2D mapTransform = entity.GetTransform();
    const glm::vec2 tileSize = entity.GetTileSize();

    if (tileSize.x <= 0.0f || tileSize.y <= 0.0f)
        return false;

    const glm::vec2 local = worldPosition - mapTransform.position;
    const int tileX = static_cast<int>(std::floor(local.x / tileSize.x));
    const int tileY = static_cast<int>(std::floor(local.y / tileSize.y));

    if (tileX < 0 || tileY < 0)
        return false;

    if (tileX >= static_cast<int>(entity.GetTileMapWidth()) ||
        tileY >= static_cast<int>(entity.GetTileMapHeight()))
    {
        return false;
    }

    outTile = {tileX, tileY};
    return true;
}

glm::vec2 SandboxApp::ScreenToWorld(const glm::vec2& screenPosition, const SceneViewportState& viewportState) const
{
    const glm::vec2 viewportMin{
        viewportState.contentMin.x,
        viewportState.contentMin.y
    };

    const glm::vec2 viewportLocal = screenPosition - viewportMin;

    return m_Scene.GetCamera().ScreenToWorld(
        viewportLocal,
        static_cast<float>(viewportState.width),
        static_cast<float>(viewportState.height)
    );
}

void SandboxApp::OnUpdate(float deltaTime)
{
    CameraCommand command{};
    glm::vec2 playerInput{0.0f, 0.0f};

    const InputState& input = GetInputState();
    const SceneViewportState& viewportState = GetSceneViewportState();
    
    if (input.CanUseEditorViewportInput() && viewportState.hovered)
    {
        auto& tileMapPanel = GetEditorLayer().GetTileMapEditorPanel();
        Entity selected = m_Scene.GetEntity(tileMapPanel.GetSelectedObjectID());

        glm::ivec2 hoveredTile{-1, -1};
        if (tileMapPanel.HasActiveTileMap(m_Scene) && TryGetHoveredTile(selected, hoveredTile))
        {
            tileMapPanel.SetHoveredTile(hoveredTile);

            if (input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && tileMapPanel.IsPaintModeEnabled())
                selected.SetTile(hoveredTile.x, hoveredTile.y, tileMapPanel.GetSelectedTileID());

            if (
                input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ||
                (tileMapPanel.IsEraseModeEnabled() && input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            )
                selected.SetTile(hoveredTile.x, hoveredTile.y, -1);
        }
        else
        {
            tileMapPanel.SetHoveredTile(glm::ivec2{-1, -1});
        }
    }

    if (input.CanUseRuntimeViewportInput())
    {
        if (input.IsKeyDown(GLFW_KEY_A))
        {
            playerInput.x -= 1.0f;
        }
        if (input.IsKeyDown(GLFW_KEY_D))
        {
            playerInput.x += 1.0f;
        }
        if (input.IsKeyDown(GLFW_KEY_W))
        {
            playerInput.y -= 1.0f;
        }
        if (input.IsKeyDown(GLFW_KEY_S))
        {
            playerInput.y += 1.0f;
        }
    }

    if (input.CanUseEditorViewportInput())
    {
        if (input.IsKeyDown(GLFW_KEY_Q))
            command.zoomDelta -= 1.0f;
        if (input.IsKeyDown(GLFW_KEY_E))
            command.zoomDelta += 1.0f;
    }

    if (input.CanUseEditorShortcuts() && input.WasKeyPressed(GLFW_KEY_F5))
        m_Scene.SaveToFile(m_SceneFilePath);

    if (input.CanUseEditorShortcuts() && input.WasKeyPressed(GLFW_KEY_F9))
    {
        if (m_Scene.LoadFromFile(m_SceneFilePath))
            RefreshRuntimeHandles();
    }

    if (m_EditorMode == EditorMode::Playing && m_Player.IsValid())
        m_Player.Move(playerInput, deltaTime);

    if (viewportState.visible)
    {
        m_Scene.UpdateCamera(
            command,
            deltaTime,
            static_cast<float>(viewportState.width),
            static_cast<float>(viewportState.height)
        );
    }

    if (m_EditorMode == EditorMode::Playing)
        m_Scene.Update(deltaTime);
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_Scene.Render(renderer);
}
