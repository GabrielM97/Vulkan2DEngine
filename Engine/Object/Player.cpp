#include "Object/Player.h"

#include <GLFW/glfw3native.h>

#include "Component/Gameplay/DebugSettingsComponent.h"
#include "Component/Gameplay/PlayerMovementComponent.h"
#include "Scene/Scene.h"

void Player::Initialize()
{
    auto& movement = GetEntity().AddComponent<PlayerMovementComponent>();
    GetEntity().AddComponent<DebugSettingsComponent>();

    SetPosition({0.0f, 0.0f});
    SetRotation(0.0f);
    movement.moveSpeed = 250.0f;

    ConfigureSprite(
        "Assets/Textures/Sprite-0001.png",
        {32.0f, 32.0f},
        glm::vec4(1.0f),
        0
    );
    
    //SetSpriteSourceRectFromGrid(0, 10);

    //EnsureAnimation();
    //SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
}

void Player::BeginPlay()
{
    if (GetEntity().HasComponent<PlayerMovementComponent>())
        GetEntity().GetComponent<PlayerMovementComponent>().runtimeVelocity = 0.0f;

    if (GetEntity().HasComponent<DebugSettingsComponent>())
        GetEntity().GetComponent<DebugSettingsComponent>().runtimeAccumulator = 0.0f;
    
    PlayAnimation("Walk");
}

void Player::Move(const glm::vec2& input, float deltaTime)
{
    auto& playerEntity = GetEntity();
    float moveSpeed = GetEntity().GetComponent<PlayerMovementComponent>().moveSpeed;
    const glm::vec2 moveDelta = input * moveSpeed* deltaTime;
    playerEntity.MoveWithCollision(moveDelta);
}

void Player::update(float deltaTime)
{
    auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();
    Move(movement.MoveDirection, deltaTime);
}

void Player::OnCollisionEnter(const OverlapResult& overlap_result)
{
    if (overlap_result.IsTile())
    {
        glm::vec2 hitpos = GetScene()->GetTileWorldPosition(overlap_result.objectID, overlap_result.tileCoordinates.x, overlap_result.tileCoordinates.y);
        const auto tiledata = GetScene()->GetEntity(overlap_result.objectID).GetTileMapData();
        float width = tiledata.width * tiledata.tileSize.x - tiledata.tileSize.x;
        float height = tiledata.height * tiledata.tileSize.y - tiledata.tileSize.y;
        auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();
        
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
    }
    
    Actor::OnCollisionEnter(overlap_result);
}

void Player::OnCollisionExit(const OverlapResult& overlap_result)
{
    Actor::OnCollisionExit(overlap_result);
}

void Player::OnCollisionBlocked(const OverlapResult& overlap_result)
{

    
    Actor::OnCollisionBlocked(overlap_result);
}
