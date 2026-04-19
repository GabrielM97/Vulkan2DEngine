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
    
    void update(float deltaTime);
    
    virtual void OnCollisionEnter(const OverlapResult&) override;
    virtual void OnCollisionExit(const OverlapResult&) override;
    virtual void OnCollisionBlocked(const OverlapResult&) override;
    
    REGISTER_SCENE_OBJECT(Player)
};
