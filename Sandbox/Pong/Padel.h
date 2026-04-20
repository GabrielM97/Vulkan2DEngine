#pragma once
#include "Object/Actor.h"
#include "Object/ObjectRegistry.h"

class Padel : public Actor
{

public:
    Padel() = default;
    using Actor::Actor;

    static const char* StaticName() { return "Padel"; }
    const char* GetTypeName() const override { return StaticName(); }

    void Initialize() override;
    void BeginPlay() override;
    void Move(const glm::vec2& input, float deltaTime);

    void update(float deltaTime);

    virtual void OnCollisionEnter(const OverlapResult&) override;
    virtual void OnCollisionStay(const OverlapResult&) override;
    virtual void OnCollisionExit(const OverlapResult&) override;
    virtual void OnCollisionBlocked(const OverlapResult&) override;

    REGISTER_SCENE_OBJECT(Padel)

};
