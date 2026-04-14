#pragma once

#include "Component/SceneComponentRegistry.h"
#include "Reflection/Property.h"
#include "Reflection/PropertyTypeTraits.h"

class PlayerMovementComponent : public SceneComponent
{
public:
    PROPERTY_FIELD(float, moveSpeed, EditAnywhere | Save, 250.0f);
    float runtimeVelocity = 0.0f;

    REGISTER_SCENE_COMPONENT(PlayerMovementComponent, 1, "PlayerMovementComponent")
};

REGISTER_PROPERTIES(
    PlayerMovementComponent,
    PROPERTY_INFO(PlayerMovementComponent, moveSpeed, EditAnywhere | Save)
);
