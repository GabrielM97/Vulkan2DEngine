#include "RegisterSceneComponents.h"

#include <memory>

#include "SceneComponent.h"
#include "SceneComponentRegistry.h"
#include "Gameplay/DebugSettingsComponent.h"
#include "Gameplay/PlayerMovementComponent.h"
#include "Scene/SceneComponents.h"

void RegisterSceneComponents()
{
    auto& registry = SceneComponentRegistry::Get();

    static bool registered = false;
    if (registered)
        return;

    registry.Register(std::make_unique<TypedSceneComponent<PlayerMovementComponent>>());
    registry.Register(std::make_unique<TypedSceneComponent<DebugSettingsComponent>>());
    registry.Register(std::make_unique<TypedSceneComponent<SpriteAnimationComponent>>());

    registered = true;
}