#pragma once

#include <string>
#include <utility>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>

#include "SceneComponents.h"

class Scene;

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

    Transform2D GetWorldTransform() const;
    LocalTransformComponent GetLocalTransform() const;
    void SetWorldTransform(const Transform2D& transform) const;
    void SetLocalTransform(const LocalTransformComponent& transform) const;

    glm::vec2 GetLocalPosition() const;
    void SetLocalPosition(const glm::vec2& position) const;
    void SetLocalRotation(float rotationDegrees) const;

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

private:
    Scene* m_Scene = nullptr;
    entt::registry* m_Registry = nullptr;
    entt::entity m_Entity{entt::null};
    GameObjectID m_ID = 0;
};

template<typename T, typename... Args>
T& Entity::AddComponent(Args&&... args) const
{
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
    if (m_Registry->all_of<T>(m_Entity))
        m_Registry->remove<T>(m_Entity);
}
