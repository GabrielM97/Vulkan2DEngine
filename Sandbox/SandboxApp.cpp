#include "SandboxApp.h"

#include <iostream>
#include <cstdint>
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

    GetEditorLayer().GetTileMapEditorPanel().SetTileMapViewEnabled(false);

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

void SandboxApp::BeginTileStroke(Entity entity)
{
    m_TileStrokeActive = true;
    m_ActiveTileStroke = {};
    m_ActiveTileStroke.targetID = entity.GetID();
}

void SandboxApp::ApplyTileStrokeStamp(Entity entity, glm::ivec2 hoveredTile, bool erase)
{
    if (!m_TileStrokeActive || !entity.IsValid() || !entity.HasTileMap())
        return;

    auto& tileMapPanel = GetEditorLayer().GetTileMapEditorPanel();
    const glm::ivec2 selectionSize = tileMapPanel.GetSelectedAtlasSize();
    const int atlasColumns = static_cast<int>(entity.GetTileMapColumns());
    const uint32_t activeLayer = entity.GetActiveTileLayerIndex();

    for (int offsetY = 0; offsetY < selectionSize.y; ++offsetY)
    {
        for (int offsetX = 0; offsetX < selectionSize.x; ++offsetX)
        {
            const int tileX = hoveredTile.x + offsetX;
            const int tileY = hoveredTile.y + offsetY;
            const int32_t newValue = erase
                ? -1
                : tileMapPanel.GetSelectedTileIDAtOffset({offsetX, offsetY}, atlasColumns);
            const int32_t oldValue = entity.GetTile(activeLayer, tileX, tileY);

            if (oldValue == newValue)
                continue;

            const int64_t key =
                (static_cast<int64_t>(tileY) << 32) |
                static_cast<uint32_t>(tileX);

            auto lookupIt = m_ActiveTileStroke.cellLookup.find(key);
            if (lookupIt == m_ActiveTileStroke.cellLookup.end())
            {
                m_ActiveTileStroke.cellLookup.emplace(key, m_ActiveTileStroke.cells.size());
                m_ActiveTileStroke.cells.push_back({
                    activeLayer,
                    tileX,
                    tileY,
                    oldValue,
                    newValue
                });
            }
            else
            {
                m_ActiveTileStroke.cells[lookupIt->second].after = newValue;
            }

            entity.SetTile(activeLayer, tileX, tileY, newValue);
        }
    }
}

void SandboxApp::EndTileStroke()
{
    if (!m_TileStrokeActive)
        return;

    m_TileStrokeActive = false;

    if (!m_ActiveTileStroke.cells.empty())
    {
        m_ActiveTileStroke.cellLookup.clear();
        m_UndoTileStrokes.push_back(std::move(m_ActiveTileStroke));
        m_RedoTileStrokes.clear();
    }
    else
    {
        m_ActiveTileStroke = {};
    }
}

void SandboxApp::UndoTileStroke()
{
    if (m_UndoTileStrokes.empty())
        return;

    TileEditStroke stroke = std::move(m_UndoTileStrokes.back());
    m_UndoTileStrokes.pop_back();

    Entity entity = m_Scene.GetEntity(stroke.targetID);
    if (entity.IsValid() && entity.HasTileMap())
    {
        for (const TileEditCell& cell : stroke.cells)
            entity.SetTile(cell.layerIndex, cell.x, cell.y, cell.before);
    }

    stroke.cellLookup.clear();
    m_RedoTileStrokes.push_back(std::move(stroke));
}

void SandboxApp::RedoTileStroke()
{
    if (m_RedoTileStrokes.empty())
        return;

    TileEditStroke stroke = std::move(m_RedoTileStrokes.back());
    m_RedoTileStrokes.pop_back();

    Entity entity = m_Scene.GetEntity(stroke.targetID);
    if (entity.IsValid() && entity.HasTileMap())
    {
        for (const TileEditCell& cell : stroke.cells)
            entity.SetTile(cell.layerIndex, cell.x, cell.y, cell.after);
    }

    stroke.cellLookup.clear();
    m_UndoTileStrokes.push_back(std::move(stroke));
}

bool SandboxApp::IsUndoShortcutPressed() const
{
    const InputState& input = GetInputState();
    const bool controlDown =
        input.IsKeyDown(GLFW_KEY_LEFT_CONTROL) ||
        input.IsKeyDown(GLFW_KEY_RIGHT_CONTROL);

    return controlDown && input.WasKeyPressed(GLFW_KEY_Z);
}

bool SandboxApp::IsRedoShortcutPressed() const
{
    const InputState& input = GetInputState();
    const bool controlDown =
        input.IsKeyDown(GLFW_KEY_LEFT_CONTROL) ||
        input.IsKeyDown(GLFW_KEY_RIGHT_CONTROL);

    return controlDown && input.WasKeyPressed(GLFW_KEY_Y);
}

void SandboxApp::OnUpdate(float deltaTime)
{
    CameraCommand command{};
    glm::vec2 playerInput{0.0f, 0.0f};

    const InputState& input = GetInputState();
    const SceneViewportState& viewportState = GetSceneViewportState();

    if (!IsEditorPlaying() && input.CanUseEditorShortcuts() && IsUndoShortcutPressed())
        UndoTileStroke();

    if (!IsEditorPlaying() && input.CanUseEditorShortcuts() && IsRedoShortcutPressed())
        RedoTileStroke();
    
    if (!IsEditorPlaying() && viewportState.visible && viewportState.hovered)
    {
        auto& tileMapPanel = GetEditorLayer().GetTileMapEditorPanel();
        Entity selected = m_Scene.GetEntity(tileMapPanel.GetSelectedObjectID());

        glm::ivec2 hoveredTile{-1, -1};
        if (tileMapPanel.HasActiveTileMap(m_Scene) && TryGetHoveredTile(selected, hoveredTile))
        {
            tileMapPanel.SetHoveredTile(hoveredTile);

            const bool paintStroke =
                input.CanUseEditorViewportInput() &&
                input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) &&
                tileMapPanel.IsPaintModeEnabled();
            const bool eraseStroke =
                input.CanUseEditorViewportInput() &&
                (input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ||
                 (tileMapPanel.IsEraseModeEnabled() && input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)));

            if ((paintStroke || eraseStroke) && !m_TileStrokeActive)
                BeginTileStroke(selected);

            if (paintStroke)
            {
                ApplyTileStrokeStamp(selected, hoveredTile, false);
            }
             
            if (eraseStroke)
                ApplyTileStrokeStamp(selected, hoveredTile, true);
        }
        else
        {
            tileMapPanel.SetHoveredTile(glm::ivec2{-1, -1});
        }
    }
    else
    {
        GetEditorLayer().GetTileMapEditorPanel().SetHoveredTile(glm::ivec2{-1, -1});
    }

    if (m_TileStrokeActive &&
        (input.WasMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT) ||
         input.WasMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT)))
    {
        EndTileStroke();
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
        if (input.IsKeyDown(GLFW_KEY_A) || input.IsKeyDown(GLFW_KEY_LEFT))
            command.moveX -= 1.0f;
        if (input.IsKeyDown(GLFW_KEY_D) || input.IsKeyDown(GLFW_KEY_RIGHT))
            command.moveX += 1.0f;
        if (input.IsKeyDown(GLFW_KEY_W) || input.IsKeyDown(GLFW_KEY_UP))
            command.moveY -= 1.0f;
        if (input.IsKeyDown(GLFW_KEY_S) || input.IsKeyDown(GLFW_KEY_DOWN))
            command.moveY += 1.0f;

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
    const TileMapEditorPanel& tileMapPanel = GetEditorLayer().GetTileMapEditorPanel();
    Entity selected = m_Scene.GetEntity(tileMapPanel.GetSelectedObjectID());

    if (tileMapPanel.IsTileMapViewEnabled() && selected.IsValid() && selected.HasTileMap())
    {
        m_Scene.RenderTileMapOnly(renderer, selected.GetID());
        return;
    }

    m_Scene.Render(renderer);
}
