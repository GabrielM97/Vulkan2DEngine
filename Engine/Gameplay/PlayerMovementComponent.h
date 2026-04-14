#pragma once

#include "Reflection/Property.h"
#include "Reflection/PropertyTypeTraits.h"

struct PlayerMovementComponent
{
    PROPERTY_FIELD(float, moveSpeed, EditAnywhere | Save, 250.0f);
    float runtimeVelocity = 0.0f;
};

REGISTER_PROPERTIES(
    PlayerMovementComponent,
    PROPERTY_INFO(PlayerMovementComponent, moveSpeed, EditAnywhere | Save)
);
