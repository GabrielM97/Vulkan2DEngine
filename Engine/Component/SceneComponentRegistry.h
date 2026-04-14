#pragma once

#include <memory>
#include <type_traits>
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
    void Ensure(Entity& entity) const override \
    { \
        if (!entity.HasComponent<Type>()) \
            entity.AddComponent<Type>(); \
    } \
    bool Has(const Entity& entity) const override \
    { \
        return entity.HasComponent<Type>(); \
    } \
    void Serialize(const Entity& entity, nlohmann::json& out) const override \
    { \
        if (!entity.HasComponent<Type>()) \
            return; \
        SerializeProperties(entity.GetComponent<Type>(), out); \
    } \
    void Deserialize(Entity& entity, const nlohmann::json& in) const override \
    { \
        auto& component = entity.AddComponent<Type>(); \
        DeserializeProperties(component, in); \
    } \
private: \
    inline static const SceneComponentAutoRegistrar<Type> s_AutoRegistrar{}; \
public:
