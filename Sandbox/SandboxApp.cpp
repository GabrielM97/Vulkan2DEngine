#include "SandboxApp.h"

#include <GLFW/glfw3.h>

void SandboxApp::OnInit()
{
    m_Player = m_Scene.CreateGameObjectHandle("Player Sprite");
    m_Player.SetLocalPosition({0.0f, 0.0f});
    m_Player.SetLocalRotation(0.0f);
    m_Player.SetSpriteTexturePath("Assets/Textures/character-spritesheet.png");
    m_Player.SetSpriteSourceRectFromGrid(0, 10, 64, 64);
    m_Player.SetSpriteTint(glm::vec4(1.0f));
    m_Player.SetSpriteLayer(0);

    if (GameObject* player = m_Scene.FindGameObjectByID(m_Player.GetID()))
    {
        player->animation.emplace();
        player->animation->SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
        player->animation->Play("Walk");
    }

    GameObjectHandle weapon = m_Scene.CreateGameObjectHandle("Weapon", m_Player.GetID());
    weapon.SetLocalPosition({50.f, 0.f});
    weapon.SetSpriteSize({16.f, 16.f});
    weapon.SetSpriteTexturePath("Assets/Textures/texture.jpg");
    weapon.SetSpriteLayer(1);

    GameObjectHandle weapon2 = m_Scene.CreateGameObjectHandle("Weapon", m_Player.GetID());
    weapon2.SetLocalPosition({-25.f, 0.f});
    weapon2.SetSpriteSize({16.f, 16.f});
    weapon2.SetSpriteTexturePath("Assets/Textures/texture.jpg");
    weapon2.SetSpriteLayer(1);
}

void SandboxApp::OnUpdate(float deltaTime)
{
    CameraCommand command{};

    if (!IsKeyboardCapturedByUI())
    {
        if (IsKeyDown(GLFW_KEY_A))
            command.moveX -= 1.0f;
        if (IsKeyDown(GLFW_KEY_D))
            command.moveX += 1.0f;
        if (IsKeyDown(GLFW_KEY_W))
            command.moveY -= 1.0f;
        if (IsKeyDown(GLFW_KEY_S))
            command.moveY += 1.0f;

        if (IsKeyDown(GLFW_KEY_Q))
            command.zoomDelta -= 1.0f;
        if (IsKeyDown(GLFW_KEY_E))
            command.zoomDelta += 1.0f;
    }

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
