#pragma once

#include <algorithm>
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

struct AABB2D
{
    glm::vec2 min{0.0f, 0.0f};
    glm::vec2 max{0.0f, 0.0f}; 
};

class IRenderer2D;
class SceneSerializer;

class Scene
{
public:
    Scene();

    Entity CreateEntity(const std::string& name = "Entity", GameObjectID parentID = 0);
    Entity GetEntity(GameObjectID id);
    Entity GetEntity(GameObjectID id) const;

    template<typename TObject, typename... Args>
    TObject Place(Args&&... args);

    template<typename TObject, typename... Args>
    TObject Spawn(Args&&... args);

    bool IsValidGameObject(GameObjectID id) const;
    bool IsValidHandle(entt::entity entity, GameObjectID id) const;

    void DestroyGameObject(GameObjectID id);
    size_t GetGameObjectCount() const;

    void Update(float deltaTime);
    void Render(IRenderer2D& renderer);
    void RenderTileMapOnly(IRenderer2D& renderer, GameObjectID tileMapID);
    void UpdateCamera(const CameraCommand& command, float deltaTime, float viewportWidth, float viewportHeight);
    bool SaveToFile(const std::string& path) const;
    bool LoadFromFile(const std::string& path);
    bool IsPlaying() const { return m_IsPlaying; }
    void BeginPlay();
    void EndPlay();

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
    
    bool OverlapsSolidBox(const glm::vec2& center, const glm::vec2& size, GameObjectID ignoredID = 0) const;
    bool OverlapsSolidBox(const AABB2D& box, GameObjectID ignoredID = 0) const;

    AABB2D BuildWorldAABB(GameObjectID id) const;
    AABB2D BuildColliderAABB(GameObjectID id) const;
    AABB2D BuildColliderAABB(GameObjectID id, const glm::vec2& overridePosition) const;
    AABB2D BuildTileAABB(GameObjectID tileMapID, int tileX, int tileY) const;
    bool Intersects(const AABB2D& a, const AABB2D& b) const;
    glm::vec2 ResolveMovement(GameObjectID id, const glm::vec2& currentPosition, const glm::vec2& delta) const;
    bool MoveWithCollision(GameObjectID id, const glm::vec2& delta);
    
    void RenderCollisionDebug(IRenderer2D& renderer, GameObjectID focusedTileMapID, bool tileMapOnly);
    
    Camera2D& GetCamera() { return m_Camera; }
    const Camera2D& GetCamera() const { return m_Camera; }

private:

    friend class Entity;
    friend class SceneSerializer;

    entt::entity CreateEntityInternal(const std::string& name);
    entt::entity CreateEntityWithID(const std::string& name, GameObjectID id);
    entt::entity FindEntityByID(GameObjectID id) const;
    void Clear();

    const SpriteAnimationSet* GetOrLoadAnimationSet(const std::string& path);
    void MarkTransformDirty(GameObjectID id);
    Transform2D ResolveWorldTransform(entt::entity entity);
    void RenderEntity(IRenderer2D& renderer, entt::entity entity);
    std::vector<entt::entity> SortForRendering() const;
    void DestroyPendingGameObjects();
    bool WouldCreateCycle(GameObjectID childID, GameObjectID parentID) const;
    void HandleChildrenOnDestroy(GameObjectID parentID);
    void DestroyGameObjectRecursive(GameObjectID id);
    void ConnectRegistrySignals();
    void OnLocalTransformUpdated(entt::registry& registry, entt::entity entity);
    void ResolveRequiredComponents(entt::entity entity);

    template<typename TObject, typename... Args>
    TObject CreateObject(bool beginPlayIfAlreadyPlaying, Args&&... args);

    entt::registry m_Registry;
    std::unordered_map<GameObjectID, entt::entity> m_EntityByID;
    Camera2D m_Camera;
    std::unordered_map<std::string, SpriteAnimationSet> m_AnimationSetCache;
    GameObjectID m_NextGameObjectID = 1;
    bool m_IsPlaying = false;
};

template<typename TObject, typename... Args>
TObject Scene::Place(Args&&... args)
{
    return CreateObject<TObject>(false, std::forward<Args>(args)...);
}

template<typename TObject, typename... Args>
TObject Scene::Spawn(Args&&... args)
{
    return CreateObject<TObject>(true, std::forward<Args>(args)...);
}

template<typename TObject, typename... Args>
TObject Scene::CreateObject(bool beginPlayIfAlreadyPlaying, Args&&... args)
{
    Entity entity = CreateEntity(TObject::StaticName());
    const entt::entity rawEntity = FindEntityByID(entity.GetID());
    m_Registry.get<ObjectTypeComponent>(rawEntity).typeName = TObject::StaticName();

    TObject object(entity);
    object.Initialize(std::forward<Args>(args)...);

    if (beginPlayIfAlreadyPlaying && m_IsPlaying)
        object.BeginPlay();

    return object;
}
