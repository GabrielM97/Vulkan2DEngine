#include "SandboxApp.h"

#include <GLFW/glfw3.h>

#include "Gameplay/PlayerMovementComponent.h"

void SandboxApp::OnInit()
{
    CreateDefaultScene();
    m_Scene.BeginPlay();
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

void SandboxApp::OnUpdate(float deltaTime)
{
    CameraCommand command{};
    glm::vec2 playerInput{0.0f, 0.0f};
    
    const SceneViewportState& viewportState = GetSceneViewportState();
    const bool allowSceneInput = viewportState.visible
        && viewportState.focused
        && IsKeyboardCapturedByUI();

    if (allowSceneInput)
    {
        if (IsKeyDown(GLFW_KEY_A))
        {
            playerInput.x -= 1.0f;
        }
        if (IsKeyDown(GLFW_KEY_D))
        {
            playerInput.x += 1.0f;
        }
        if (IsKeyDown(GLFW_KEY_W))
        {
            playerInput.y -= 1.0f;
        }
        if (IsKeyDown(GLFW_KEY_S))
        {
            playerInput.y += 1.0f;
        }

        if (IsKeyDown(GLFW_KEY_Q))
            command.zoomDelta -= 1.0f;
        if (IsKeyDown(GLFW_KEY_E))
            command.zoomDelta += 1.0f;
    }

    const bool savePressed = IsKeyDown(GLFW_KEY_F5);
    if (savePressed && !m_SavePressedLastFrame)
        m_Scene.SaveToFile(m_SceneFilePath);
    m_SavePressedLastFrame = savePressed;

    const bool loadPressed = IsKeyDown(GLFW_KEY_F9);
    if (loadPressed && !m_LoadPressedLastFrame)
    {
        if (m_Scene.LoadFromFile(m_SceneFilePath))
        {
            RefreshRuntimeHandles();
            m_Scene.BeginPlay();
        }
    }
    m_LoadPressedLastFrame = loadPressed;

    if (m_Player.IsValid())
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

    m_Scene.Update(deltaTime);
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_Scene.Render(renderer);
}
