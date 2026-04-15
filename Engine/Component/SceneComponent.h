#pragma once

#include <nlohmann/json.hpp>

#include "EntityComponent.h"

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
