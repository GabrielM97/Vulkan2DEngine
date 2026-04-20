#pragma once

#include "Component/SceneComponentRegistry.h"
#include "Reflection/Property.h"
#include "Reflection/PropertyTypeTraits.h"

class PlayerMovementComponent : public SceneComponent
{
public:
    PROPERTY_FIELD(float, moveSpeed, EditAnywhere | Save, 250.0f);
    PROPERTY_FIELD(glm::vec2, MoveDirection, EditAnywhere | Save, glm::vec2(1.0f, 1.0f));
    float runtimeVelocity = 0.0f;
    
    bool colliding = false;
    
    REGISTER_SCENE_COMPONENT(PlayerMovementComponent, 1, "PlayerMovementComponent")
};

REGISTER_PROPERTIES(
    PlayerMovementComponent,
    PROPERTY_INFO(PlayerMovementComponent, moveSpeed, EditAnywhere | Save),
    PROPERTY_INFO(PlayerMovementComponent, MoveDirection, EditAnywhere | Save)
);
