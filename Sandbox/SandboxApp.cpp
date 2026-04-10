#include "SandboxApp.h"

#include <algorithm>
#include <GLFW/glfw3.h>

void SandboxApp::OnInit()
{
    uint32_t m_TextureA = GetRenderer().LoadTexture("Assets/Textures/texture.jpg");
    uint32_t m_TextureB = GetRenderer().LoadTexture("Assets/Textures/texture2.jpg");
    
    GameObject& first = m_Scene.CreateGameObject();
    first.transform.position = {50.0f, 50.0f};
    first.transform.size = {128.0f, 128.0f};
    first.transform.rotationDegrees = 0.0f;
    first.sprite.textureIndex = m_TextureA;
    first.sprite.tint = glm::vec4(1.0f);
    first.sprite.layer = 0;

    GameObject& second = m_Scene.CreateGameObject();
    second.transform.position = {240.0f, 50.0f};
    second.transform.size = {128.0f, 128.0f};
    second.transform.rotationDegrees = 0.0f;
    second.sprite.textureIndex = m_TextureB;
    second.sprite.tint = glm::vec4(1.0f);
    second.sprite.layer = 1;

    GameObject& transparent = m_Scene.CreateGameObject();
    transparent.transform.position = {300.0f, 50.0f};
    transparent.transform.size = {128.0f, 128.0f};
    transparent.transform.rotationDegrees = 0.0f;
    transparent.sprite.textureIndex = m_TextureA;
    transparent.sprite.tint = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);
    transparent.sprite.layer = 2;

    GameObject& third = m_Scene.CreateGameObject();
    third.transform.position = {430.0f, 50.0f};
    third.transform.size = {128.0f, 128.0f};
    third.transform.rotationDegrees = 0.0f;
    third.sprite.textureIndex = m_TextureA;
    third.sprite.tint = glm::vec4(1.0f);
    third.sprite.layer = 0;
}

void SandboxApp::OnUpdate(float deltaTime)
{
    constexpr float cameraSpeed = 400.0f;
    constexpr float zoomSpeed = 1.5f;

    if (IsKeyDown(GLFW_KEY_A))
        m_Camera.position.x += cameraSpeed * deltaTime;

    if (IsKeyDown(GLFW_KEY_D))
        m_Camera.position.x -= cameraSpeed * deltaTime;

    if (IsKeyDown(GLFW_KEY_W))
        m_Camera.position.y += cameraSpeed * deltaTime;

    if (IsKeyDown(GLFW_KEY_S))
        m_Camera.position.y -= cameraSpeed * deltaTime;

    if (IsKeyDown(GLFW_KEY_Q))
        m_Camera.zoom -= zoomSpeed * deltaTime;

    if (IsKeyDown(GLFW_KEY_E))
        m_Camera.zoom += zoomSpeed * deltaTime;

    m_Camera.zoom = std::clamp(m_Camera.zoom, 0.25f, 4.0f);

    GetRenderer().SetCamera(m_Camera);


    auto& objects = m_Scene.GetGameObjects();

    // Tiny test so we know Sandbox owns game behavior.
    if (!objects.empty())
    {
        for (auto& object : objects)
            object.transform.rotationDegrees += 45.0f * deltaTime;
    }
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    m_Scene.Render(renderer);
}