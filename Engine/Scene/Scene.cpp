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

        renderer.DrawQuad(
            object->transform.position,
            object->transform.size,
            object->transform.rotationDegrees,
            object->sprite.tint,
            object->sprite.textureIndex
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