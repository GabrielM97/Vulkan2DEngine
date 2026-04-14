#pragma once

#include "EntityComponent.h"
#include "Gameplay/DebugSettingsComponent.h"
#include "Gameplay/PlayerMovementComponent.h"
#include "Scene/SceneComponents.h"

template<typename T>
struct SceneComponentTraits
{
    static constexpr bool Trackable = false;
    static constexpr ComponentTypeID ID = 0;
    static constexpr const char* Name = "UnregisteredComponent";
};

template<>
struct SceneComponentTraits<PlayerMovementComponent>
{
    static constexpr bool Trackable = true;
    static constexpr ComponentTypeID ID = 1;
    static constexpr const char* Name = "PlayerMovementComponent";
};

template<>
struct SceneComponentTraits<DebugSettingsComponent>
{
    static constexpr bool Trackable = true;
    static constexpr ComponentTypeID ID = 2;
    static constexpr const char* Name = "DebugSettingsComponent";
};

template<>
struct SceneComponentTraits<SpriteAnimationComponent>
{
    static constexpr bool Trackable = true;
    static constexpr ComponentTypeID ID = 3;
    static constexpr const char* Name = "SpriteAnimationComponent";
};
