#include "SandboxApp.h"

#include <algorithm>

void SandboxApp::OnInit()
{
    GameObject first{};
    first.transform.position = {50.0f, 50.0f};
    first.transform.size = {128.0f, 128.0f};
    first.transform.rotationDegrees = 0.0f;
    first.sprite.textureIndex = 0;
    first.sprite.tint = glm::vec4(1.0f);
    first.sprite.layer = 0;

    GameObject second{};
    second.transform.position = {240.0f, 50.0f};
    second.transform.size = {128.0f, 128.0f};
    second.transform.rotationDegrees = 25.0f;
    second.sprite.textureIndex = 1;
    second.sprite.tint = glm::vec4(1.0f);
    second.sprite.layer = 1;

    GameObject transparent{};
    transparent.transform.position = {300.0f, 50.0f};
    transparent.transform.size = {128.0f, 128.0f};
    transparent.transform.rotationDegrees = 25.0f;
    transparent.sprite.textureIndex = 0;
    transparent.sprite.tint = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);
    transparent.sprite.layer = 2;

    GameObject third{};
    third.transform.position = {430.0f, 50.0f};
    third.transform.size = {128.0f, 128.0f};
    third.transform.rotationDegrees = -15.0f;
    third.sprite.textureIndex = 0;
    third.sprite.tint = glm::vec4(1.0f);
    third.sprite.layer = 0;

    m_GameObjects.push_back(first);
    m_GameObjects.push_back(second);
    m_GameObjects.push_back(transparent);
    m_GameObjects.push_back(third);
}

void SandboxApp::OnUpdate(float deltaTime)
{
    // Tiny test so we know Sandbox owns game behavior.
    if (!m_GameObjects.empty())
    {
        m_GameObjects[0].transform.rotationDegrees += 45.0f * deltaTime;
    }
}

void SandboxApp::OnRender(VulkanRenderer& renderer)
{
    // Stable sort preserves creation order for objects on the same layer.
    std::stable_sort(
        m_GameObjects.begin(),
        m_GameObjects.end(),
        [](const GameObject& a, const GameObject& b)
        {
            return a.sprite.layer < b.sprite.layer;
        }
    );

    for (const GameObject& object : m_GameObjects)
    {
        renderer.DrawQuad(
            object.transform.position,
            object.transform.size,
            object.transform.rotationDegrees,
            object.sprite.tint,
            object.sprite.textureIndex
        );
    }
}