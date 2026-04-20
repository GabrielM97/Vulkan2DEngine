#include "Ball.h"

#include "Component/Gameplay/PlayerMovementComponent.h"
#include "Scene/Scene.h"

void Ball::Initialize()
{
    auto& movemnt = GetEntity().AddComponent<PlayerMovementComponent>();
    movemnt.moveSpeed = 550.0f;
    ConfigureSprite(
     "Assets/Textures/Sprite-0001.png",
     {32.0f, 32.0f},
     glm::vec4(1.0f),
     0
 );
}

void Ball::BeginPlay()
{
    Actor::BeginPlay();
}

void Ball::Move(const glm::vec2& input, float deltaTime)
{
   
}

void Ball::update(float deltaTime)
{
    auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();
    const glm::vec2 moveDelta = movement.MoveDirection * movement.moveSpeed* deltaTime;
    GetEntity().MoveWithCollision(moveDelta);
}

void Ball::OnCollisionEnter(const OverlapResult& overlap_result)
{
    
    Actor::OnCollisionEnter(overlap_result);
    
    auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();
    
    if (overlap_result.IsTile())
    {
        glm::vec2 hitpos = GetScene()->GetTileWorldPosition(overlap_result.objectID, overlap_result.tileCoordinates.x, overlap_result.tileCoordinates.y);
        const auto tiledata = GetScene()->GetEntity(overlap_result.objectID).GetTileMapData();
        float width = tiledata.width * tiledata.tileSize.x - tiledata.tileSize.x;
        float height = tiledata.height * tiledata.tileSize.y - tiledata.tileSize.y;
        
        if (hitpos.x <= 0.f )
        {
            movement.MoveDirection.x = 1.0f;
        }
        if (hitpos.x >= width)
        {
            movement.MoveDirection.x = -1.0f;
        }
        if (hitpos.y <= 0.0f)
        {
            movement.MoveDirection.y = 1.0f;
        }
        if (hitpos.y >= height)
        {
            movement.MoveDirection.y = -1.0f;
        }
        
        return;
    }
    
    Transform2D transform = GetScene()->GetEntity(overlap_result.objectID).GetWorldTransform();

    if (transform.position.x < 480.f)
    {
        movement.MoveDirection.x = 1.0f;
        movement.moveSpeed += 50.f; 
    }
    else
    {
        movement.MoveDirection.x = -1.0f;
    }
    
}

void Ball::OnCollisionExit(const OverlapResult& overlap_result)
{
    Actor::OnCollisionExit(overlap_result);
}

void Ball::OnCollisionBlocked(const OverlapResult& overlap_result)
{
    Actor::OnCollisionBlocked(overlap_result);
}
