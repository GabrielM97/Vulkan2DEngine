#pragma once

#include <string>

#include "Scene/Entity.h"

class Object
{
public:
    Object() = default;
    explicit Object(Entity entity)
        : m_Entity(entity)
    {
    }
    
    virtual ~Object() = default;
    virtual const char* GetTypeName() const = 0;

    GameObjectID GetID() const { return m_Entity.GetID(); }
    bool IsValid() const { return m_Entity.IsValid(); }

    std::string GetName() const { return m_Entity.GetName(); }
    void SetName(const std::string& name) { m_Entity.SetName(name); }

    bool IsActive() const { return m_Entity.IsActive(); }
    void SetActive(bool active) { m_Entity.SetActive(active); }

    void Destroy() { m_Entity.Destroy(); }
    
    Entity& GetEntity() { return m_Entity; }
    const Entity& GetEntity() const { return m_Entity; }

private:
    Entity m_Entity;
};
