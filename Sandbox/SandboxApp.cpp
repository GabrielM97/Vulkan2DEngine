#include "SandboxApp.h"

#include <algorithm>
#include <GLFW/glfw3.h>

void SandboxApp::OnInit()
{
    GameObject& first = m_Scene.CreateGameObject("First Sprite");
    first.transform.position = {50.0f, 50.0f};
    first.transform.size = {128.0f, 128.0f};
    first.transform.rotationDegrees = 0.0f;
    first.sprite.texture.path = "Assets/Textures/texture.jpg";
    first.sprite.tint = glm::vec4(1.0f);
    first.sprite.layer = 0;

    GameObject& second = m_Scene.CreateGameObject("Second Sprite");
    second.transform.position = {240.0f, 50.0f};
    second.transform.size = {128.0f, 128.0f};
    second.transform.rotationDegrees = 0.0f;
    second.sprite.texture.path = "Assets/Textures/texture2.jpg";
    second.sprite.tint = glm::vec4(1.0f);
    second.sprite.layer = 1;

    GameObject& transparent = m_Scene.CreateGameObject("Transparent Sprite");
    transparent.transform.position = {300.0f, 50.0f};
    transparent.transform.size = {128.0f, 128.0f};
    transparent.transform.rotationDegrees = 0.0f;
    transparent.sprite.texture.path = "Assets/Textures/texture.jpg";
    transparent.sprite.tint = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);
    transparent.sprite.layer = 2;

    GameObject& third = m_Scene.CreateGameObject("Third Sprite");
    third.transform.position = {430.0f, 50.0f};
    third.transform.size = {128.0f, 128.0f};
    third.transform.rotationDegrees = 0.0f;
    third.sprite.texture.path = "Assets/Textures/cute pixel art cat_21719083.png";
    third.sprite.tint = glm::vec4(1.0f);
    third.sprite.layer = 0;
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
    
    for (size_t i = 0; i < m_Scene.GetGameObjectCount(); ++i)
    {
        if (GameObject* object = m_Scene.GetGameObject(i))
        {
            object->transform.rotationDegrees += 45.0f * deltaTime;
        }
    }
    m_Scene.Update(deltaTime);
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_Scene.Render(renderer);
}