#include "SandboxApp.h"

#include <algorithm>
#include <GLFW/glfw3.h>

void SandboxApp::OnInit()
{
    GameObject& Player = m_Scene.CreateGameObject("Player Sprite");
    Player.transform.position = {300.0f, 50.0f};
    Player.transform.size = {128.0f, 128.0f};
    Player.transform.rotationDegrees = 0.0f;
    Player.sprite.SetTexturePath("Assets/Textures/testspritesheet.png");
    Player.sprite.SetSourceRect(0,0, 128, 128);
    Player.sprite.SetTint(glm::vec4(1.0f, 1.0f, 1.0f, 0.6f));
    Player.sprite.SetLayer(0);
}

void SandboxApp::OnUpdate(float deltaTime)
{
    CameraCommand command{};

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
    
    m_Scene.UpdateCamera(command, deltaTime);

    GetRenderer().SetCamera(m_Scene.GetCamera());
    
    m_Scene.Update(deltaTime);
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_Scene.Render(renderer);
}