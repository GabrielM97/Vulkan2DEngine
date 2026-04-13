#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <entt/entt.hpp>

#include "Camera2D.h"
#include "Entity.h"
#include "SceneComponents.h"
#include "SpriteAnimationSet.h"

struct CameraCommand
{
    float moveX = 0.0f;
    float moveY = 0.0f;
    float zoomDelta = 0.0f;
};

class IRenderer2D;

class Scene
{
public:
    Scene();

    Entity CreateEntity(const std::string& name = "Entity", GameObjectID parentID = 0);
    Entity GetEntity(GameObjectID id);

    template<typename TObject, typename... Args>
    TObject Spawn(Args&&... args);

    bool IsValidGameObject(GameObjectID id) const;
    bool IsValidHandle(entt::entity entity, GameObjectID id) const;

    void DestroyGameObject(GameObjectID id);
    size_t GetGameObjectCount() const;

    void Update(float deltaTime);
    void Render(IRenderer2D& renderer);
    void UpdateCamera(const CameraCommand& command, float deltaTime, float viewportWidth, float viewportHeight);

    std::string GetGameObjectName(GameObjectID id) const;
    bool SetGameObjectName(GameObjectID id, const std::string& name);

    bool IsGameObjectActive(GameObjectID id) const;
    bool SetGameObjectActive(GameObjectID id, bool active);

    std::vector<GameObjectID> GetGameObjectIDs() const;
    std::vector<GameObjectID> GetRootGameObjects() const;
    std::vector<GameObjectID> GetChildGameObjects(GameObjectID parentID) const;
    bool HasChildren(GameObjectID parentID) const;
    bool HasParent(GameObjectID id) const;
    GameObjectID GetParentID(GameObjectID id) const;
    bool SetParent(GameObjectID childID, GameObjectID parentID);
    bool ClearParent(GameObjectID childID);
    bool SetSiblingOrder(GameObjectID id, int order);

    ChildDestroyPolicy GetChildDestroyPolicy(GameObjectID id) const;
    bool SetChildDestroyPolicy(GameObjectID id, ChildDestroyPolicy policy);

    Transform2D GetWorldTransform(GameObjectID id) const;
    bool SetWorldTransform(GameObjectID id, const Transform2D& transform);

    LocalTransformComponent GetLocalTransform(GameObjectID id) const;
    bool SetLocalTransform(GameObjectID id, const LocalTransformComponent& transform);
    glm::vec2 GetLocalPosition(GameObjectID id) const;
    glm::vec2 GetLocalScale(GameObjectID id) const;
    glm::vec2 GetLocalPivot(GameObjectID id) const;
    float GetLocalRotation(GameObjectID id) const;
    glm::vec2 GetWorldPosition(GameObjectID id) const;
    glm::vec2 GetWorldScale(GameObjectID id) const;
    glm::vec2 GetWorldPivot(GameObjectID id) const;
    float GetWorldRotation(GameObjectID id) const;

    bool SetLocalPosition(GameObjectID id, const glm::vec2& position);
    bool SetLocalScale(GameObjectID id, const glm::vec2& scale);
    bool SetLocalPivot(GameObjectID id, const glm::vec2& pivot);
    bool SetLocalRotation(GameObjectID id, float rotationDegrees);
    bool SetWorldPosition(GameObjectID id, const glm::vec2& position);
    bool SetWorldScale(GameObjectID id, const glm::vec2& scale);
    bool SetWorldPivot(GameObjectID id, const glm::vec2& pivot);
    bool SetWorldRotation(GameObjectID id, float rotationDegrees);

    Camera2D& GetCamera() { return m_Camera; }
    const Camera2D& GetCamera() const { return m_Camera; }

private:
    friend class Entity;

    entt::entity CreateEntityInternal(const std::string& name);
    entt::entity FindEntityByID(GameObjectID id) const;

    const SpriteAnimationSet* GetOrLoadAnimationSet(const std::string& path);
    void MarkTransformDirty(GameObjectID id);
    Transform2D ResolveWorldTransform(entt::entity entity);
    std::vector<entt::entity> SortForRendering() const;
    void DestroyPendingGameObjects();
    bool WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const;
    void HandleChildrenOnDestroy(GameObjectID parentID);
    void DestroyGameObjectRecursive(GameObjectID id);
    void ConnectRegistrySignals();
    void OnLocalTransformUpdated(entt::registry& registry, entt::entity entity);

    entt::registry m_Registry;
    std::unordered_map<GameObjectID, entt::entity> m_EntityByID;
    Camera2D m_Camera;
    std::unordered_map<std::string, SpriteAnimationSet> m_AnimationSetCache;
    GameObjectID m_NextGameObjectID = 1;
};

template<typename TObject, typename... Args>
TObject Scene::Spawn(Args&&... args)
{
    Entity entity = CreateEntity(TObject::StaticName());
    TObject object(entity);
    object.Initialize(std::forward<Args>(args)...);
    return object;
}
