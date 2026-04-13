#include "Entity.h"

#include "Scene.h"

bool Entity::IsValid() const
{
    return m_Scene != nullptr && m_Scene->IsValidHandle(m_Entity, m_ID);
}

std::string Entity::GetName() const
{
    return IsValid() ? m_Scene->GetGameObjectName(m_ID) : std::string{};
}

void Entity::SetName(const std::string& name) const
{
    if (IsValid())
        m_Scene->SetGameObjectName(m_ID, name);
}

bool Entity::IsActive() const
{
    return IsValid() && m_Scene->IsGameObjectActive(m_ID);
}

void Entity::SetActive(bool active) const
{
    if (IsValid())
        m_Scene->SetGameObjectActive(m_ID, active);
}

void Entity::Destroy() const
{
    if (IsValid())
        m_Scene->DestroyGameObject(m_ID);
}

bool Entity::HasParent() const
{
    return IsValid() && m_Scene->HasParent(m_ID);
}

GameObjectID Entity::GetParentID() const
{
    return IsValid() ? m_Scene->GetParentID(m_ID) : 0;
}

void Entity::SetParent(GameObjectID parentID) const
{
    if (IsValid())
        m_Scene->SetParent(m_ID, parentID);
}

void Entity::ClearParent() const
{
    if (IsValid())
        m_Scene->ClearParent(m_ID);
}

Transform2D Entity::GetWorldTransform() const
{
    return IsValid() ? m_Scene->GetWorldTransform(m_ID) : Transform2D{};
}

LocalTransformComponent Entity::GetLocalTransform() const
{
    return IsValid() ? m_Scene->GetLocalTransform(m_ID) : LocalTransformComponent{};
}

void Entity::SetWorldTransform(const Transform2D& transform) const
{
    if (IsValid())
        m_Scene->SetWorldTransform(m_ID, transform);
}

void Entity::SetLocalTransform(const LocalTransformComponent& transform) const
{
    if (IsValid())
        m_Scene->SetLocalTransform(m_ID, transform);
}

glm::vec2 Entity::GetLocalPosition() const
{
    return IsValid() ? m_Scene->GetLocalPosition(m_ID) : glm::vec2{};
}

void Entity::SetLocalPosition(const glm::vec2& position) const
{
    if (IsValid())
        m_Scene->SetLocalPosition(m_ID, position);
}

void Entity::SetLocalRotation(float rotationDegrees) const
{
    if (IsValid())
        m_Scene->SetLocalRotation(m_ID, rotationDegrees);
}
