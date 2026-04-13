#include "SandboxApp.h"

#include <GLFW/glfw3.h>

#include "Gameplay/PlayerMovementComponent.h"

void SandboxApp::OnInit()
{
    m_Player = m_Scene.Spawn<Player>();
    m_Player.SetPosition(glm::vec2{300.0f, 100.0f});

    Entity weapon = m_Scene.CreateEntity("Weapon", m_Player.GetID());
    weapon.GetComponent<LocalTransformComponent>().position = {50.f, 0.f};
    weapon.GetComponent<SpriteComponent>().SetSize({16.f, 16.f});
    weapon.GetComponent<SpriteComponent>().SetTexturePath("Assets/Textures/texture.jpg");
    weapon.GetComponent<SpriteComponent>().SetLayer(1);
    weapon.SetLocalTransform(weapon.GetComponent<LocalTransformComponent>());

    Entity weapon2 = m_Scene.CreateEntity("Weapon", m_Player.GetID());
    weapon2.GetComponent<LocalTransformComponent>().position = {-25.f, 0.f};
    weapon2.GetComponent<SpriteComponent>().SetSize({16.f, 16.f});
    weapon2.GetComponent<SpriteComponent>().SetTexturePath("Assets/Textures/texture.jpg");
    weapon2.GetComponent<SpriteComponent>().SetLayer(1);
    weapon2.SetLocalTransform(weapon2.GetComponent<LocalTransformComponent>());
}

void SandboxApp::OnUpdate(float deltaTime)
{
    CameraCommand command{};
    glm::vec2 playerInput{0.0f, 0.0f};

    if (!IsKeyboardCapturedByUI())
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

    if (m_Player.IsValid())
        m_Player.Move(playerInput, deltaTime);

    m_Scene.UpdateCamera(
        command,
        deltaTime,
        static_cast<float>(GetRenderer().GetFramebufferWidth()),
        static_cast<float>(GetRenderer().GetFramebufferHeight())
    );

    m_Scene.Update(deltaTime);
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_Scene.Render(renderer);
}
