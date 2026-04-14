#pragma once

#include <nlohmann/json.hpp>

#include "EntityComponent.h"
#include "Reflection/PropertySerialization.h"
#include "SceneComponentTraits.h"
#include "Scene/Entity.h"

class Entity;

class SceneComponent : public EntityComponent
{
public:
    ~SceneComponent() override = default;
    
    virtual void Ensure(Entity& entity) const = 0;
    virtual bool Has(const Entity& entity) const = 0;
    virtual void Serialize(const Entity& entity, nlohmann::json& out) const = 0;
    virtual void Deserialize(Entity& entity, const nlohmann::json& in) const = 0;
};

template<typename TComponent>
class TypedSceneComponent : public SceneComponent
{
public:
    ComponentTypeID GetID() const override
    {
        return SceneComponentTraits<TComponent>::ID;
    }

    const char* GetTypeName() const override
    {
        return SceneComponentTraits<TComponent>::Name;
    }
    
    void Ensure(Entity& entity) const override
    {
        if (!entity.HasComponent<TComponent>())
            entity.AddComponent<TComponent>();
    }

    bool Has(const Entity& entity) const override
    {
        return entity.HasComponent<TComponent>();
    }
    
    void Serialize(const Entity& entity, nlohmann::json& out) const override
    {
        if (!entity.HasComponent<TComponent>())
            return;

        SerializeProperties(entity.GetComponent<TComponent>(), out);
    }

    void Deserialize(Entity& entity, const nlohmann::json& in) const override
    {
        auto& component = entity.AddComponent<TComponent>();
        DeserializeProperties(component, in);
    }
};
