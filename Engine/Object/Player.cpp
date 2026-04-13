#include "Object/Player.h"
#include "Scene/SceneComponents.h"
#include "Gameplay/PlayerMovementComponent.h"

void Player::Initialize()
{
    auto& sprite = GetEntity().GetComponent<SpriteComponent>();
    auto& animation = GetEntity().AddComponent<SpriteAnimationComponent>();
    auto& movement = GetEntity().AddComponent<PlayerMovementComponent>();

    SetPosition({0.0f, 0.0f});
    SetRotation(0.0f);
    movement.moveSpeed = 250.0f;

    sprite.SetTexturePath("Assets/Textures/character-spritesheet.png");
    sprite.SetSourceRectFromGrid(0, 10, 64, 64);
    sprite.SetTint(glm::vec4(1.0f));
    sprite.SetLayer(0);

    animation.SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
    animation.Play("Walk");
}

void Player::Move(const glm::vec2& input, float deltaTime)
{
    const auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();

    glm::vec2 newPosition = GetPosition() + input * movement.moveSpeed * deltaTime;
    SetPosition(newPosition);
}
