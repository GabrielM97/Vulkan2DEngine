#include "Scene.h"

#include "Renderer/VulkanRenderer.h"

#include <algorithm>

GameObject& Scene::CreateGameObject()
{
    return m_GameObjects.emplace_back();
}

void Scene::Render(VulkanRenderer& renderer)
{
    SortForRendering();

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

std::vector<GameObject>& Scene::GetGameObjects()
{
    return m_GameObjects;
}

const std::vector<GameObject>& Scene::GetGameObjects() const
{
    return m_GameObjects;
}

void Scene::SortForRendering()
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
}
