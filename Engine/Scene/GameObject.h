#pragma once

#include <optional>

#include "Components.h"
#include "SpriteRenderer.h"
#include "SpriteAnimation.h"

#include <string>

class GameObject
{
public:
    std::string name = "GameObject";
    bool active = true;
    bool pendingDestroy = false;

    Transform2D transform;
    SpriteRenderer sprite;
    
    std::optional<SpriteAnimation> animation;
};
