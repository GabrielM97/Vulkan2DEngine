#include "Scene.h"

#include "Renderer/VulkanRenderer.h"
#include "Camera2D.h"

#include <algorithm>

GameObject& Scene::CreateGameObject(const std::string& name)
{
    auto object = std::make_unique<GameObject>();
    object->name = name;

    GameObject& reference = *object;
    m_GameObjects.push_back(std::move(object));
    return reference;
}

void Scene::DestroyGameObject(GameObject& object)
{
    object.pendingDestroy = true;
    object.active = false;
}

size_t Scene::GetGameObjectCount() const
{
    return m_GameObjects.size();
}

GameObject* Scene::GetGameObject(size_t index)
{
    if (index >= m_GameObjects.size())
        return nullptr;

    return m_GameObjects[index].get();
}

const GameObject* Scene::GetGameObject(size_t index) const
{
    if (index >= m_GameObjects.size())
        return nullptr;

    return m_GameObjects[index].get();
}

GameObject* Scene::FindGameObjectByName(const std::string& name)
{
    for (const auto& object : m_GameObjects)
    {
        if (object->name == name)
            return object.get();
    }

    return nullptr;
}

const GameObject* Scene::FindGameObjectByName(const std::string& name) const
{
    for (const auto& object : m_GameObjects)
    {
        if (object->name == name)
            return object.get();
    }

    return nullptr;
}

void Scene::DestroyPendingGameObjects()
{
    std::erase_if(
      m_GameObjects,
      [](const std::unique_ptr<GameObject>& object)
      {
          return object->pendingDestroy;
      }
  );
}

void Scene::Render(VulkanRenderer& renderer)
{
    SortForRendering();

    for (const std::unique_ptr<GameObject>& object : m_GameObjects)
    {
        if (!object->active || !object->sprite.visible)
            continue;

        uint32_t textureIndex = renderer.GetOrLoadTexture(object->sprite.texture.path);

        glm::vec2 uvMin{0.0f, 0.0f};
        glm::vec2 uvMax{1.0f, 1.0f};

        if (object->sprite.useSourceRect)
        {
            if (const VulkanTexture* texture = renderer.GetTexture(textureIndex))
            {
                const float textureWidth = static_cast<float>(texture->GetWidth());
                const float textureHeight = static_cast<float>(texture->GetHeight());

                if (textureWidth > 0.0f && textureHeight > 0.0f)
                {
                    const IntRect& rect = object->sprite.sourceRect;

                    uvMin.x = static_cast<float>(rect.x) / textureWidth;
                    uvMin.y = static_cast<float>(rect.y) / textureHeight;

                    uvMax.x = static_cast<float>(rect.x + rect.width) / textureWidth;
                    uvMax.y = static_cast<float>(rect.y + rect.height) / textureHeight;
                }
            }
        }

        renderer.DrawQuad(
            object->transform.position,
            object->transform.size,
            object->transform.rotationDegrees,
            uvMin,
            uvMax,
            object->sprite.tint,
            textureIndex
        );
    }
}

void Scene::Update(float deltaTime)
{
    for (const std::unique_ptr<GameObject>& object : m_GameObjects)
    {
        if (!object->active)
            continue;

        // Later this is where engine-level object/component updates can happen.
        // For now, Scene update mainly gives us a safe cleanup point.
    }

    DestroyPendingGameObjects();
}

void Scene::UpdateCamera(const CameraCommand& command, float deltaTime)
{
    m_Camera.Update(
        command.moveX,
        command.moveY,
        command.zoomDelta,
        deltaTime
    );
}

std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects()
{
    return m_GameObjects;
}

const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const
{
    return m_GameObjects;
}

void Scene::SortForRendering()
{
    // Stable sort preserves creation order for objects on the same layer.
    std::stable_sort(
        m_GameObjects.begin(),
        m_GameObjects.end(),
        [](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b)
        {
            return a->sprite.layer < b->sprite.layer;
        }
    );
}