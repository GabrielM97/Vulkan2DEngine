#pragma once

#include <span>

#include <nlohmann/json.hpp>

#include "EntityComponent.h"
#include "Reflection/Property.h"

class Entity;

class SceneComponent : public EntityComponent
{
public:
    ~SceneComponent() override = default;
    
    virtual void Ensure(Entity& entity) const = 0;
    virtual void Remove(Entity& entity) const = 0;
    virtual bool Has(const Entity& entity) const = 0;
    virtual void Serialize(const Entity& entity, nlohmann::json& out) const = 0;
    virtual void Deserialize(Entity& entity, const nlohmann::json& in) const = 0;
    virtual std::span<const Property> GetProperties() const = 0;
    virtual void* GetMutableData(Entity& entity) const = 0;
    virtual const void* GetData(const Entity& entity) const = 0;
};
