#include "Object/Player.h"

#include "Gameplay/PlayerMovementComponent.h"

void Player::Initialize()
{
    auto& movement = GetEntity().AddComponent<PlayerMovementComponent>();

    SetPosition({0.0f, 0.0f});
    SetRotation(0.0f);
    movement.moveSpeed = 250.0f;

    ConfigureSprite(
        "Assets/Textures/character-spritesheet.png",
        {64.0f, 64.0f},
        glm::vec4(1.0f),
        0
    );
    
    SetSpriteSourceRectFromGrid(0, 10);

    EnsureAnimation();
    SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
    PlayAnimation("Walk");
}

void Player::Move(const glm::vec2& input, float deltaTime)
{
    const auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();

    glm::vec2 newPosition = GetPosition() + input * movement.moveSpeed * deltaTime;
    SetPosition(newPosition);
}
