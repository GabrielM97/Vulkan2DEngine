#include "Padel.h"

#include "Component/Gameplay/PlayerMovementComponent.h"

void Padel::Initialize()
{
    auto& movement = GetEntity().AddComponent<PlayerMovementComponent>();
    
    movement.MoveDirection = glm::vec2(0.0f, 0.0f);
    movement.moveSpeed = 450.0f;
    
    GetEntity().EnsureBoxCollider();
    GetEntity().SetBoxColliderSize({16.f, 128.f});
    GetEntity().SetColliderBodyType(ColliderBodyType::Dynamic);
    
    ConfigureSprite(
     "Assets/Textures/Sprite-0002.png",
     {16.0f, 64.0f},
     glm::vec4(1, 0,0, 1),
     0);
    
    SetScale(glm::vec2(1.0f, 2.0f));
}

void Padel::BeginPlay()
{
    Actor::BeginPlay();
    auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();
    movement.MoveDirection = glm::vec2(0.0f, 0.0f);
}

void Padel::Move(const glm::vec2& input, float deltaTime)
{
}

void Padel::update(float deltaTime)
{
    
    auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();
    
    const glm::vec2 moveDelta = movement.MoveDirection * movement.moveSpeed* deltaTime;
    GetEntity().MoveWithCollision(moveDelta);
    
}

void Padel::OnCollisionEnter(const OverlapResult& overlap_result)
{
    if (overlap_result.IsTile())
    {
        auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();
        movement.MoveDirection = glm::vec2(0.0f, 0.0f);
    }
  
    Actor::OnCollisionEnter(overlap_result);
}

void Padel::OnCollisionStay(const OverlapResult& overlap_result)
{
 
    Actor::OnCollisionStay(overlap_result);
}

void Padel::OnCollisionExit(const OverlapResult& overlap_result)
{
    
}

void Padel::OnCollisionBlocked(const OverlapResult& overlap_result)
{
    Actor::OnCollisionBlocked(overlap_result);
}
