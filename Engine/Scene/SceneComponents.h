#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Math/Transform2D.h"
#include "Gameplay/PlayerMovementComponent.h"
#include "SpriteAnimation.h"
#include "SpriteRenderer.h"

using GameObjectID = uint64_t;
using LocalTransformComponent = Transform2D;
using SpriteComponent = SpriteRenderer;
using SpriteAnimationComponent = SpriteAnimation;

enum class RequiredComponentID : uint32_t
{
    SpriteAnimation = 1,
    PlayerMovement = 2
};

template<typename T>
struct RequiredComponentTraits
{
    static constexpr bool Trackable = false;
};

template<>
struct RequiredComponentTraits<SpriteAnimationComponent>
{
    static constexpr bool Trackable = true;
    static constexpr RequiredComponentID ID = RequiredComponentID::SpriteAnimation;
};

template<>
struct RequiredComponentTraits<PlayerMovementComponent>
{
    static constexpr bool Trackable = true;
    static constexpr RequiredComponentID ID = RequiredComponentID::PlayerMovement;
};

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
    std::vector<RequiredComponentID> componentIDs;
};
