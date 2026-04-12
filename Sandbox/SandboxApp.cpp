#include "SandboxApp.h"

#include <algorithm>
#include <GLFW/glfw3.h>

void SandboxApp::OnInit()
{
    GameObject& Player = m_Scene.CreateGameObject("Player Sprite");
    m_PlayerID = Player.GetID();
    Player.transform.position = {0.0f, 0.0f};
    Player.transform.rotationDegrees = 0.0f;
    Player.sprite.SetTexturePath("Assets/Textures/character-spritesheet.png");
    Player.sprite.SetSourceRectFromGrid(0,10, 64, 64);
    Player.sprite.SetTint(glm::vec4(1.0f));
    Player.sprite.SetLayer(0);
    Player.animation.emplace();
    Player.animation->SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
    Player.animation->Play("Walk");
    
    GameObject& Weapon = m_Scene.CreateGameObject("Weapon", m_PlayerID);
    Weapon.transform.position = {50.f, 0.f};
    Weapon.sprite.SetSize(16.f, 16.f);
    Weapon.sprite.SetTexturePath("Assets/Textures/texture.jpg");
    Weapon.sprite.SetLayer(1);
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
    
    if (GameObject* player = m_Scene.FindGameObjectByID(m_PlayerID))
    {
       //player->transform.position.y += 25.0f * deltaTime;
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

    if (GameObject* player = m_Scene.FindGameObjectByID(m_PlayerID))
    {
        ImGui::Separator();
        ImGui::Text("Player");
        ImGui::DragFloat2("Position", &player->transform.position.x, 1.0f);
        ImGui::DragFloat2("Scale", &player->transform.scale.x, 0.01f, 0.0f, 100.0f);
        ImGui::DragFloat2("Pivot", &player->transform.pivot.x, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Rotation", &player->transform.rotationDegrees, 1.0f);
    }

    ImGui::End();
}
