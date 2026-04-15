#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "SpriteAnimation.h"
#include "Math/Transform2D.h"
#include "SpriteRenderer.h"
#include "Component/EntityComponent.h"

using GameObjectID = uint64_t;
using LocalTransformComponent = Transform2D;
using SpriteComponent = SpriteRenderer;
using SpriteAnimationComponent = SpriteAnimation;

enum class ChildDestroyPolicy
{
    DetachToRoot,
    DestroyWithParent
};

struct IDComponent
{
    GameObjectID id = 0;
};

struct NameComponent
{
    std::string name = "GameObject";
};

struct ActiveComponent
{
    bool active = true;
    bool pendingDestroy = false;
};

struct RelationshipComponent
{
    GameObjectID parentID = 0;
    int siblingOrder = 0;
    ChildDestroyPolicy childDestroyPolicy = ChildDestroyPolicy::DetachToRoot;
};

struct WorldTransformComponent
{
    Transform2D world{};
    bool dirty = true;
};

struct RequiredComponentsComponent
{
    std::vector<ComponentTypeID> componentIDs;
};

struct ObjectTypeComponent
{
    std::string typeName;
};
