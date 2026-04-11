#include "SandboxApp.h"

#include <algorithm>
#include <GLFW/glfw3.h>

void SandboxApp::OnInit()
{
    GameObject& Player = m_Scene.CreateGameObject("Player Sprite");
    m_PlayerID = Player.GetID();
    Player.transform.position = {-300.0f, 0.0f};
    Player.transform.size = {64.0f, 64.0f};
    Player.transform.rotationDegrees = 0.0f;
    Player.sprite.SetTexturePath("Assets/Textures/character-spritesheet.png");
    Player.sprite.SetSourceRectFromGrid(0,10, 64, 64);
    Player.sprite.SetTint(glm::vec4(1.0f));
    Player.sprite.SetLayer(0);
    Player.animation.emplace();
    Player.animation->SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
    Player.animation->Play("Walk");
    
    GameObject& Weapon = m_Scene.CreateGameObject("Weapon");
    Weapon.transform.position = {0.f, 0.f};
    Weapon.transform.size = {16.f, 16.f};
    Weapon.sprite.SetTexturePath("Assets/Textures/texture.jpg");
    Weapon.sprite.SetLayer(1);
    
    m_Scene.SetParent(Weapon.GetID(), m_PlayerID);
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
        player->transform.position.x += 25.0f * deltaTime;
    }
    
    m_Scene.UpdateCamera(command, deltaTime);

    GetRenderer().SetCamera(m_Scene.GetCamera());
    
    m_Scene.Update(deltaTime);
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_Scene.Render(renderer);
}

void SandboxApp::OnImGuiUpdate()
{
    ImGui::Begin("Debug");
    ImGui::Text("ImGui is working.");
    ImGui::Text("Objects: %d", 1);
    ImGui::End();
}
