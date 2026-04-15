#pragma once

#include <glm/glm.hpp>

#include "Actor.h"
#include "ObjectRegistry.h"

class Player : public Actor
{
public:
    Player() = default;
    using Actor::Actor;

    static const char* StaticName() { return "Player"; }
    const char* GetTypeName() const override { return StaticName(); }

    void Initialize() override;
    void BeginPlay() override;
    void Move(const glm::vec2& input, float deltaTime);

    REGISTER_SCENE_OBJECT(Player)
};
