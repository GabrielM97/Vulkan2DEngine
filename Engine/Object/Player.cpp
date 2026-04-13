#include "Object/Player.h"
#include "Scene/SceneComponents.h"
#include "Gameplay/PlayerMovementComponent.h"

void Player::Initialize()
{
    auto& transform = GetEntity().GetComponent<LocalTransformComponent>();
    auto& sprite = GetEntity().GetComponent<SpriteComponent>();
    auto& animation = GetEntity().AddComponent<SpriteAnimationComponent>();
    auto& movement = GetEntity().AddComponent<PlayerMovementComponent>();

    transform.position = {0.0f, 0.0f};
    transform.rotationDegrees = 0.0f;
    movement.moveSpeed = 250.0f;

    sprite.SetTexturePath("Assets/Textures/character-spritesheet.png");
    sprite.SetSourceRectFromGrid(0, 10, 64, 64);
    sprite.SetTint(glm::vec4(1.0f));
    sprite.SetLayer(0);

    animation.SetAnimationSetPath("Assets/Animations/CharacterSpriteSheet.csv");
    animation.Play("Walk");

    GetEntity().SetLocalTransform(transform);
}

void Player::Move(const glm::vec2& input, float deltaTime)
{
    auto transform = GetEntity().GetLocalTransform();
    const auto& movement = GetEntity().GetComponent<PlayerMovementComponent>();

    transform.position += input * movement.moveSpeed * deltaTime;
    GetEntity().SetLocalTransform(transform);
}
