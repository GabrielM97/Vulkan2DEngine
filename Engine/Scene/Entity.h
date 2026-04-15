#pragma once

#include <string>
#include <type_traits>
#include <utility>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

#include "SceneComponents.h"

class Scene;

template<typename T, typename = void>
struct HasStaticSceneComponentID : std::false_type
{
};

template<typename T>
struct HasStaticSceneComponentID<T, std::void_t<decltype(T::StaticComponentID)>> : std::true_type
{
};

class Entity
{
public:
    Entity() = default;
    Entity(Scene* scene, entt::registry* registry, entt::entity entity, GameObjectID id)
        : m_Scene(scene), m_Registry(registry), m_Entity(entity), m_ID(id)
    {
    }

    bool IsValid() const;
    explicit operator bool() const { return IsValid(); }

    GameObjectID GetID() const { return m_ID; }

    std::string GetName() const;
    void SetName(const std::string& name) const;

    bool IsActive() const;
    void SetActive(bool active) const;

    void Destroy() const;

    bool HasParent() const;
    GameObjectID GetParentID() const;
    void SetParent(GameObjectID parentID) const;
    void ClearParent() const;
    ChildDestroyPolicy GetChildDestroyPolicy() const;
    void SetChildDestroyPolicy(ChildDestroyPolicy policy) const;
    
    Transform2D GetTransform() const;
    void SetTransform(const Transform2D& transform) const;

    glm::vec2 GetPosition() const;
    void SetPosition(const glm::vec2& position) const;
    
    float GetRotation() const;
    void SetRotation(float rotationDegrees) const;
    
    glm::vec2 GetScale() const;
    void SetScale(const glm::vec2& scale) const;
    
    glm::vec2 GetLocalPosition() const;
    void SetLocalPosition(const glm::vec2& position) const;
    
    float GetLocalRotation() const;
    void SetLocalRotation(float rotationDegrees) const;
    
    glm::vec2 GetLocalScale() const;
    void SetLocalScale(const glm::vec2& scale) const;
    
    Transform2D GetWorldTransform() const;
    void SetWorldTransform(const Transform2D& transform) const;

    LocalTransformComponent GetLocalTransform() const;
    void SetLocalTransform(const LocalTransformComponent& transform) const;

    std::string GetSpriteTexturePath() const;
    IntRect GetSpriteSourceRect() const;
    bool SpriteUsesSourceRect() const;
    glm::vec2 GetSpriteSize() const;
    glm::vec4 GetSpriteTint() const;
    int GetSpriteLayer() const;
    bool IsSpriteVisible() const;
    bool IsSpriteFlippedX() const;
    bool IsSpriteFlippedY() const;
    void SetSpriteTexturePath(const std::string& path);
    void SetSpriteSourceRect(int x, int y, int width, int height);
    void SetSpriteSourceRectFromGrid(int column, int row, int cellWidth, int cellHeight);
    void SetSpriteSourceRectFromGrid(int column, int row);
    void ClearSpriteSourceRect();
    void SetSpriteSize(const glm::vec2& size);
    void SetSpriteTint(const glm::vec4& tint);
    void SetSpriteLayer(int layer);
    void SetSpriteVisible(bool visible);
    void SetSpriteFlipX(bool flip);
    void SetSpriteFlipY(bool flip);

    bool HasAnimation() const;
    void EnsureAnimation() const;
    void RemoveAnimation() const;
    std::string GetAnimationSetPath() const;
    std::string GetAnimationClipName() const;
    void SetAnimationSetPath(const std::string& path) const;
    void PlayAnimation(const std::string& clipName, bool restartIfSame = false) const;
    void StopAnimation();
    void ResetAnimation();
    bool IsAnimationPlaying() const;
    bool HasAnimationFinished() const;
    bool IsPlayingAnimationClip(const std::string& clipName) const;
    
    bool HasTileMap() const;
    void EnsureTileMap() const;
    void RemoveTileMap() const;

    uint32_t GetTileMapWidth() const;
    uint32_t GetTileMapHeight() const;
    glm::vec2 GetTileSize() const;
    void SetTileSize(const glm::vec2& size) const;
    glm::ivec2 GetTileAtlasCellSize() const;
    void SetTileAtlasCellSize(const glm::ivec2& size) const;

    std::string GetTileMapTexturePath() const;
    void SetTileMapTexturePath(const std::string& path) const;

    uint32_t GetTileMapColumns() const;
    uint32_t GetTileMapRows() const;
    void SetTileMapGrid(uint32_t columns, uint32_t rows) const;

    void ResizeTileMap(uint32_t width, uint32_t height) const;
    int32_t GetTile(int x, int y) const;
    void SetTile(int x, int y, int32_t tileID) const;
    
    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) const;

    template<typename T>
    bool HasComponent() const;

    template<typename T>
    T& GetComponent();

    template<typename T>
    const T& GetComponent() const;

    template<typename T>
    void RemoveComponent() const;
    
    const std::vector<ComponentTypeID>& GetTrackedComponentIDs() const;

private:
    void RegisterTrackedComponent(ComponentTypeID componentID) const;
    void UnregisterTrackedComponent(ComponentTypeID componentID) const;

    Scene* m_Scene = nullptr;
    entt::registry* m_Registry = nullptr;
    entt::entity m_Entity{entt::null};
    GameObjectID m_ID = 0;
};

template<typename T, typename... Args>
T& Entity::AddComponent(Args&&... args) const
{
    if constexpr (HasStaticSceneComponentID<T>::value)
        RegisterTrackedComponent(T::StaticComponentID);

    if (m_Registry->all_of<T>(m_Entity))
        return m_Registry->get<T>(m_Entity);

    return m_Registry->emplace<T>(m_Entity, std::forward<Args>(args)...);
}

template<typename T>
bool Entity::HasComponent() const
{
    return m_Registry != nullptr && m_Registry->valid(m_Entity) && m_Registry->all_of<T>(m_Entity);
}

template<typename T>
T& Entity::GetComponent()
{
    static_assert(
        !std::is_same_v<T, IDComponent> &&
        !std::is_same_v<T, NameComponent> &&
        !std::is_same_v<T, ActiveComponent> &&
        !std::is_same_v<T, RelationshipComponent> &&
        !std::is_same_v<T, LocalTransformComponent> &&
        !std::is_same_v<T, WorldTransformComponent> &&
        !std::is_same_v<T, RequiredComponentsComponent> &&
        !std::is_same_v<T, ObjectTypeComponent> &&
        !std::is_same_v<T, SpriteComponent> &&
        !std::is_same_v<T, SpriteAnimationComponent>&&
        !std::is_same_v<T, TileMapComponent>,
        "Use Scene or Entity APIs for engine-managed components."
    );

    return m_Registry->get<T>(m_Entity);
}

template<typename T>
const T& Entity::GetComponent() const
{
    return m_Registry->get<T>(m_Entity);
}

template<typename T>
void Entity::RemoveComponent() const
{
    if constexpr (HasStaticSceneComponentID<T>::value)
        UnregisterTrackedComponent(T::StaticComponentID);

    if (m_Registry->all_of<T>(m_Entity))
        m_Registry->remove<T>(m_Entity);
}
