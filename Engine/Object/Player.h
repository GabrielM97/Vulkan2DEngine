#pragma once

#include <glm/glm.hpp>

#include "Actor.h"

class Player : public Actor
{
public:
    Player() = default;
    using Actor::Actor;

    static const char* StaticName() { return "Player"; }
    const char* GetTypeName() const override { return StaticName(); }

    void Initialize() override;
    void Move(const glm::vec2& input, float deltaTime);
};
