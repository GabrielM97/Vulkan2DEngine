#pragma once

#include <memory>
#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <vector>

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

    std::vector<const SceneComponent*> GetAll() const
    {
        std::vector<const SceneComponent*> components;
        components.reserve(m_Components.size());

        for (const auto& [id, component] : m_Components)
        {
            (void)id;
            components.push_back(component.get());
        }

        std::sort(
            components.begin(),
            components.end(),
            [](const SceneComponent* lhs, const SceneComponent* rhs)
            {
                return lhs->GetID() < rhs->GetID();
            }
        );

        return components;
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
    void Remove(Entity& entity)
    {
        if (entity.HasComponent<TComponent>())
            entity.RemoveComponent<TComponent>();
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

    template<typename TComponent>
    std::span<const Property> GetProperties()
    {
        return PropertyRegistry<TComponent>::Get();
    }

    template<typename TComponent>
    void* GetMutableData(Entity& entity)
    {
        if (!entity.HasComponent<TComponent>())
            return nullptr;

        return &entity.GetComponent<TComponent>();
    }

    template<typename TComponent>
    const void* GetData(const Entity& entity)
    {
        if (!entity.HasComponent<TComponent>())
            return nullptr;

        return &entity.GetComponent<TComponent>();
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
    void Remove(Entity& entity) const override { SceneComponentDetail::Remove<Type>(entity); } \
    bool Has(const Entity& entity) const override { return SceneComponentDetail::Has<Type>(entity); } \
    void Serialize(const Entity& entity, nlohmann::json& out) const override { SceneComponentDetail::Serialize<Type>(entity, out); } \
    void Deserialize(Entity& entity, const nlohmann::json& in) const override { SceneComponentDetail::Deserialize<Type>(entity, in); } \
    std::span<const Property> GetProperties() const override { return SceneComponentDetail::GetProperties<Type>(); } \
    void* GetMutableData(Entity& entity) const override { return SceneComponentDetail::GetMutableData<Type>(entity); } \
    const void* GetData(const Entity& entity) const override { return SceneComponentDetail::GetData<Type>(entity); } \
private: \
    inline static const SceneComponentAutoRegistrar<Type> s_AutoRegistrar{}; \
public:
