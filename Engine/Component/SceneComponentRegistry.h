#pragma once
#include <memory>
#include <unordered_map>

#include "EntityComponent.h"
#include "SceneComponent.h"

class SceneComponentRegistry
{
public:
    static SceneComponentRegistry& Get()
    {
        static SceneComponentRegistry instance;
        return instance;
    }

    void Register(std::unique_ptr<SceneComponent> component)
    {
        if (!component)
            return;

        m_Components[component->GetID()] = std::move(component);
    }

    const SceneComponent* Find(ComponentTypeID id) const
    {
        auto it = m_Components.find(id);
        return it != m_Components.end() ? it->second.get() : nullptr;
    }

private:
    std::unordered_map<ComponentTypeID, std::unique_ptr<SceneComponent>> m_Components;
};
