#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>

#include "EntityComponent.h"
#include "Reflection/PropertySerialization.h"
#include "Scene/Entity.h"
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

namespace SceneComponentDetail
{
    template<typename TComponent>
    void Ensure(Entity& entity)
    {
        if (!entity.HasComponent<TComponent>())
            entity.AddComponent<TComponent>();
    }

    template<typename TComponent>
    bool Has(const Entity& entity)
    {
        return entity.HasComponent<TComponent>();
    }

    template<typename TComponent>
    void Serialize(const Entity& entity, nlohmann::json& out)
    {
        if (!entity.HasComponent<TComponent>())
            return;

        SerializeProperties(entity.GetComponent<TComponent>(), out);
    }

    template<typename TComponent>
    void Deserialize(Entity& entity, const nlohmann::json& in)
    {
        auto& component = entity.AddComponent<TComponent>();
        DeserializeProperties(component, in);
    }
}

template<typename TComponent>
struct SceneComponentAutoRegistrar
{
    SceneComponentAutoRegistrar()
    {
        static_assert(std::is_base_of_v<SceneComponent, TComponent>,
            "REGISTER_SCENE_COMPONENT requires the type to derive from SceneComponent.");

        SceneComponentRegistry::Get().Register(std::make_unique<TComponent>());
    }
};

#define REGISTER_SCENE_COMPONENT(Type, IdValue, NameValue) \
public: \
    static constexpr ComponentTypeID StaticComponentID = IdValue; \
    static constexpr const char* StaticComponentName = NameValue; \
    ComponentTypeID GetID() const override { return StaticComponentID; } \
    const char* GetTypeName() const override { return StaticComponentName; } \
    void Ensure(Entity& entity) const override { SceneComponentDetail::Ensure<Type>(entity); } \
    bool Has(const Entity& entity) const override { return SceneComponentDetail::Has<Type>(entity); } \
    void Serialize(const Entity& entity, nlohmann::json& out) const override { SceneComponentDetail::Serialize<Type>(entity, out); } \
    void Deserialize(Entity& entity, const nlohmann::json& in) const override { SceneComponentDetail::Deserialize<Type>(entity, in); } \
private: \
    inline static const SceneComponentAutoRegistrar<Type> s_AutoRegistrar{}; \
public:
