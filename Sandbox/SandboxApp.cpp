#include "SandboxApp.h"

#include <GLFW/glfw3.h>

#include "Gameplay/PlayerMovementComponent.h"

void SandboxApp::OnInit()
{
    CreateDefaultScene();
}

void SandboxApp::CreateDefaultScene()
{
    m_Player = m_Scene.Place<Player>();
    //m_Player.SetPosition(glm::vec2{300.0f, 100.0f});

    m_Scene.Place<Player>();
    
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

void SandboxApp::OnUpdate(float deltaTime)
{
    CameraCommand command{};
    glm::vec2 playerInput{0.0f, 0.0f};

    const InputState& input = GetInputState();
    const SceneViewportState& viewportState = GetSceneViewportState();

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
