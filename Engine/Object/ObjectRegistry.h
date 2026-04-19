#pragma once

#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "Object.h"

struct ObjectLifecycleHooks
{
    std::function<void(Entity)> beginPlay;
    std::function<void(Entity)> endPlay;
    std::function<void(Entity, const OverlapResult&)> collisionEnter;
    std::function<void(Entity, const OverlapResult&)> collisionStay;
    std::function<void(Entity, const OverlapResult&)> collisionExit;
    std::function<void(Entity, const OverlapResult&)> collisionBlocked;
    std::function<void(Entity, const OverlapResult&)> triggerEnter;
    std::function<void(Entity, const OverlapResult&)> triggerStay;
    std::function<void(Entity, const OverlapResult&)> triggerExit;
};

class ObjectRegistry
{
public:
    static ObjectRegistry& Get()
    {
        static ObjectRegistry instance;
        return instance;
    }

    void Register(const std::string& typeName, ObjectLifecycleHooks hooks)
    {
        if (typeName.empty())
            return;

        m_Hooks[typeName] = std::move(hooks);
    }

    const ObjectLifecycleHooks* Find(const std::string& typeName) const
    {
        auto it = m_Hooks.find(typeName);
        return it != m_Hooks.end() ? &it->second : nullptr;
    }

private:
    std::unordered_map<std::string, ObjectLifecycleHooks> m_Hooks;
};

template<typename TObject>
struct ObjectAutoRegistrar
{
    ObjectAutoRegistrar()
    {
        static_assert(std::is_base_of_v<Object, TObject>,
            "REGISTER_SCENE_OBJECT requires the type to derive from Object.");

        ObjectRegistry::Get().Register(
            TObject::StaticName(),
            {
                [](Entity entity)
                {
                    TObject(entity).BeginPlay();
                },
                [](Entity entity)
                {
                    TObject(entity).EndPlay();
                },
                [](Entity entity, const OverlapResult& overlap)
                {
                    TObject(entity).OnCollisionEnter(overlap);
                },
                [](Entity entity, const OverlapResult& overlap)
                {
                    TObject(entity).OnCollisionStay(overlap);
                },
                [](Entity entity, const OverlapResult& overlap)
                {
                    TObject(entity).OnCollisionExit(overlap);
                },
                [](Entity entity, const OverlapResult& overlap)
                {
                    TObject(entity).OnCollisionBlocked(overlap);
                },
                [](Entity entity, const OverlapResult& overlap)
                {
                    TObject(entity).OnTriggerEnter(overlap);
                },
                [](Entity entity, const OverlapResult& overlap)
                {
                    TObject(entity).OnTriggerStay(overlap);
                },
                [](Entity entity, const OverlapResult& overlap)
                {
                    TObject(entity).OnTriggerExit(overlap);
                }
            }
        );
    }
};

#define REGISTER_SCENE_OBJECT(Type) \
private: \
    inline static const ObjectAutoRegistrar<Type> s_ObjectAutoRegistrar{}; \
public:
