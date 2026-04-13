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

Transform2D Entity::GetTransform() const
{
    return GetWorldTransform();
}

void Entity::SetTransform(const Transform2D& transform) const
{
    SetWorldTransform(transform);   
}

glm::vec2 Entity::GetPosition() const
{
    return GetWorldTransform().position;  
}

void Entity::SetPosition(const glm::vec2& position) const
{
    Transform2D transform = GetWorldTransform();
    transform.position = position;
    SetWorldTransform(transform);
}

float Entity::GetRotation() const
{
    return GetWorldTransform().rotationDegrees;  
}

void Entity::SetRotation(float rotationDegrees) const
{
    Transform2D transform = GetWorldTransform();
    transform.rotationDegrees = rotationDegrees;
    SetWorldTransform(transform);
}

glm::vec2 Entity::GetScale() const
{
    return GetWorldTransform().scale; 
}

void Entity::SetScale(const glm::vec2& scale) const
{
    Transform2D transform = GetWorldTransform();
    transform.scale = scale;
    SetWorldTransform(transform);
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

float Entity::GetLocalRotation() const
{
    return IsValid() ? m_Scene->GetLocalRotation(m_ID) : 0;
}

void Entity::SetLocalRotation(float rotationDegrees) const
{
    if (IsValid())
        m_Scene->SetLocalRotation(m_ID, rotationDegrees);
}

glm::vec2 Entity::GetLocalScale() const
{
    return IsValid() ? m_Scene->GetLocalScale(m_ID) : glm::vec2{};
}

void Entity::SetLocalScale(const glm::vec2& scale) const
{
    if (IsValid())
        m_Scene->SetLocalScale(m_ID, scale);
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
