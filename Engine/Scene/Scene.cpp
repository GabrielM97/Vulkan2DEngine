#include "Scene.h"

#include "Renderer/IRenderer2D.h"
#include "Camera2D.h"

#include <algorithm>
#include <iostream>

namespace
{
    glm::vec2 RotateVector(const glm::vec2& value, float degrees)
    {
        const float radians = glm::radians(degrees);
        const float c = std::cos(radians);
        const float s = std::sin(radians);

        return {
            value.x * c - value.y * s,
            value.x * s + value.y * c
        };
    }

    glm::vec2 SafeDivide(const glm::vec2& value, const glm::vec2& divisor)
    {
        glm::vec2 result = value;

        if (divisor.x != 0.0f)
            result.x /= divisor.x;
        if (divisor.y != 0.0f)
            result.y /= divisor.y;

        return result;
    }
}

GameObject& Scene::CreateGameObject(const std::string& name, GameObjectID parentID)
{
    auto object = std::make_unique<GameObject>();
    object->SetID(m_NextGameObjectID++);
    object->SetName(name);
    
    if (parentID != 0 && FindGameObjectByID(parentID) != nullptr)
        object->SetParentID(parentID);

    GameObject& reference = *object;
    m_GameObjects.push_back(std::move(object));
    return reference;
}

void Scene::DestroyGameObject(GameObject& object)
{
    object.Destroy();
}

void Scene::DestroyGameObject(GameObjectID id)
{
    if (GameObject* object = FindGameObjectByID(id))
        DestroyGameObject(*object);
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
        if (object->GetName() == name)
            return object.get();
    }

    return nullptr;
}

GameObject* Scene::FindGameObjectByID(GameObjectID id)
{
    for (const auto& object : m_GameObjects)
    {
        if (object->GetID() == id)
            return object.get();
    }

    return nullptr;
}

const GameObject* Scene::FindGameObjectByID(GameObjectID id) const
{
    for (const auto& object : m_GameObjects)
    {
        if (object->GetID() == id)
            return object.get();
    }

    return nullptr;
}

const GameObject* Scene::FindGameObjectByName(const std::string& name) const
{
    for (const auto& object : m_GameObjects)
    {
        if (object->GetName() == name)
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
          return object->isPendingDestroy();
      }
  );
}

bool Scene::WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const
{
    if (parentID == 0)
        return false;

    if (parentID == childID)
        return true;

    const GameObject* parent = FindGameObjectByID(parentID);
    if (parent == nullptr)
        return false;

    return WouldCreateCycle(childID, parent->GetParentID());
}

const SpriteAnimationSet* Scene::GetOrLoadAnimationSet(const std::string& path)
{
    if (path.empty())
        return nullptr;

    auto it = m_AnimationSetCache.find(path);
    if (it != m_AnimationSetCache.end())
        return &it->second;

    SpriteAnimationSet animationSet;
    if (!animationSet.LoadFromCSV(path))
    {
        std::cerr << "Failed to load animation set '" << path << "'\n";
        return nullptr;
    }

    auto [insertedIt, inserted] = m_AnimationSetCache.emplace(path, std::move(animationSet));
    return &insertedIt->second;
}

void Scene::Render(IRenderer2D& renderer)
{
    renderer.SetCamera(m_Camera);
    const std::vector<const GameObject*> renderQueue = SortForRendering();
    
    for (const GameObject* object : renderQueue)
    {
        if (!object->isActive() || !object->sprite.IsVisible())
            continue;

        Transform2D worldTransform = GetWorldTransform(object->GetID());
        
        renderer.DrawSprite(
            worldTransform,
            object->sprite
        );
    }
}

void Scene::Update(float deltaTime)
{
    for (const std::unique_ptr<GameObject>& object : m_GameObjects)
    {
        if (!object->isActive())
            continue;

        if (object->animation.has_value())
        {
            const SpriteAnimationSet* animationSet =
                GetOrLoadAnimationSet(object->animation->GetAnimationSetRef().path);

            object->animation->Update(deltaTime, object->sprite, animationSet);
        }
    }

    DestroyPendingGameObjects();
}

void Scene::UpdateCamera(const CameraCommand& command, float deltaTime, float viewportWidth, float viewportHeight)
{
    m_Camera.Update(
        command.moveX,
        command.moveY,
        command.zoomDelta,
        deltaTime,
        viewportWidth,
        viewportHeight
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

bool Scene::SetParent(GameObjectID childID, GameObjectID parentID)
{
    if (childID == 0 || parentID == 0 || childID == parentID)
        return false;

    if (WouldCreateCycle(childID, parentID))
        return false;
    
    GameObject* child = FindGameObjectByID(childID);
    GameObject* parent = FindGameObjectByID(parentID);

    if (child == nullptr || parent == nullptr)
        return false;

    Transform2D childWorld = GetWorldTransform(childID);
    Transform2D parentWorld = GetWorldTransform(parentID);

    child->SetParentID(parentID);

    glm::vec2 localPosition = childWorld.position - parentWorld.position;
    localPosition = RotateVector(localPosition, -parentWorld.rotationDegrees);
    localPosition = SafeDivide(localPosition, parentWorld.scale);

    child->transform.position = localPosition;
    child->transform.rotationDegrees = childWorld.rotationDegrees - parentWorld.rotationDegrees;
    child->transform.scale = SafeDivide(childWorld.scale, parentWorld.scale);
    return true;
}

bool Scene::ClearParent(GameObjectID childID)
{
    GameObject* child = FindGameObjectByID(childID);
    
    if (child == nullptr)
        return false;

    Transform2D world = GetWorldTransform(childID);

    child->ClearParent();
    child->transform.position = world.position;
    child->transform.rotationDegrees = world.rotationDegrees;
    child->transform.scale = world.scale;

    return true;
}

Transform2D Scene::GetWorldTransform(GameObjectID id) const
{
      const GameObject* object = FindGameObjectByID(id);
    if (object == nullptr)
        return {};

    Transform2D world = object->transform;
    GameObjectID currentParentID = object->GetParentID();

    while (currentParentID != 0)
    {
        const GameObject* parent = FindGameObjectByID(currentParentID);
        if (parent == nullptr)
            break;

        glm::vec2 scaledLocal = world.position * parent->transform.scale;
        glm::vec2 rotatedLocal = RotateVector(scaledLocal, parent->transform.rotationDegrees);

        world.position = parent->transform.position + rotatedLocal;
        world.rotationDegrees += parent->transform.rotationDegrees;
        world.scale *= parent->transform.scale;

        currentParentID = parent->GetParentID();
    }

    return world;
}

std::vector<const GameObject*> Scene::SortForRendering()
{
    std::vector<const GameObject*> renderQueue;
    renderQueue.reserve(m_GameObjects.size());

    for (const auto& object : m_GameObjects)
    {
        if (!object->isActive() || !object->sprite.IsVisible())
            continue;

        renderQueue.push_back(object.get());
    }

    std::stable_sort(
        renderQueue.begin(),
        renderQueue.end(),
        [](const GameObject* a, const GameObject* b)
        {
            return a->sprite.GetLayer() < b->sprite.GetLayer();
        }
    );

    return renderQueue;
}
