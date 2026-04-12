#include "SandboxApp.h"

#include <algorithm>
#include <GLFW/glfw3.h>

void SandboxApp::OnInit()
{
    GameObject& Player = m_Scene.CreateGameObject("Player Sprite");
    Player.transform.scale = {100.0f, 100.0f};
    m_PlayerID = Player.GetID();
    m_Scene.SetLocalPosition(m_PlayerID, {0.0f, 0.0f});
    m_Scene.SetLocalRotation(m_PlayerID, 0.0f);
    m_Scene.SetSpriteTexturePath(m_PlayerID, "Assets/Textures/character-spritesheet.png");
    m_Scene.SetSpriteSourceRectFromGrid(m_PlayerID, 0, 10, 64, 64);
    m_Scene.SetSpriteTint(m_PlayerID, glm::vec4(1.0f));
    m_Scene.SetSpriteLayer(m_PlayerID, 0);
    Player.animation.emplace();
    Player.animation->SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
    Player.animation->Play("Walk");
    
    GameObject& Weapon = m_Scene.CreateGameObject("Weapon", m_PlayerID);
    m_Scene.SetLocalPosition(Weapon.GetID(), {50.f, 0.f});
    m_Scene.SetSpriteSize(Weapon.GetID(), {16.f, 16.f});
    m_Scene.SetSpriteTexturePath(Weapon.GetID(), "Assets/Textures/texture.jpg");
    m_Scene.SetSpriteLayer(Weapon.GetID(), 1);
    
    GameObject& Weapon2 = m_Scene.CreateGameObject("Weapon", m_PlayerID);
    m_Scene.SetLocalPosition(Weapon2.GetID(), {-25.f, 0.f});
    m_Scene.SetSpriteSize(Weapon2.GetID(), {16.f, 16.f});
    m_Scene.SetSpriteTexturePath(Weapon2.GetID(), "Assets/Textures/texture.jpg");
    m_Scene.SetSpriteLayer(Weapon2.GetID(), 1);
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

    if (m_Scene.FindGameObjectByID(m_PlayerID) != nullptr)
    {
        ImGui::Separator();
        ImGui::Text("Player");
        Transform2D localTransform = m_Scene.GetLocalTransform(m_PlayerID);

        bool transformChanged = false;
        transformChanged |= ImGui::DragFloat2("Position", &localTransform.position.x, 1.0f);
        transformChanged |= ImGui::DragFloat2("Scale", &localTransform.scale.x, 0.01f, 0.0f, 100.0f);
        transformChanged |= ImGui::DragFloat2("Pivot", &localTransform.pivot.x, 0.01f, 0.0f, 1.0f);
        transformChanged |= ImGui::DragFloat("Rotation", &localTransform.rotationDegrees, 1.0f);

        if (transformChanged)
            m_Scene.SetLocalTransform(m_PlayerID, localTransform);
    }

    ImGui::End();
}
