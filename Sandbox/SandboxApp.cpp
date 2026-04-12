#include "SandboxApp.h"

#include <algorithm>
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

    ImGuiIO& io = ImGui::GetIO();

    if (!io.WantCaptureKeyboard)
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

void SandboxApp::OnImGuiUpdate()
{
    ImGui::Begin("Debug");

    ImGui::Text("Game Objects: %zu", m_Scene.GetGameObjectCount());
    ImGui::Text("Framebuffer: %d x %d",
        GetRenderer().GetFramebufferWidth(),
        GetRenderer().GetFramebufferHeight());

    if (m_Player)
    {
        ImGui::Separator();
        ImGui::Text("Player");
        Transform2D localTransform = m_Player.GetLocalTransform();

        bool transformChanged = false;
        transformChanged |= ImGui::DragFloat2("Position", &localTransform.position.x, 1.0f);
        transformChanged |= ImGui::DragFloat2("Scale", &localTransform.scale.x, 0.01f, 0.0f, 100.0f);
        transformChanged |= ImGui::DragFloat2("Pivot", &localTransform.pivot.x, 0.01f, 0.0f, 1.0f);
        transformChanged |= ImGui::DragFloat("Rotation", &localTransform.rotationDegrees, 1.0f, -360.0f, 360.0f);

        if (transformChanged)
            m_Player.SetLocalTransform(localTransform);
    }

    ImGui::End();
}
