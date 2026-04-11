#pragma once

#include <cstdint>
#include <optional>

#include "Components.h"
#include "SpriteRenderer.h"
#include "SpriteAnimation.h"

#include <string>

using GameObjectID = uint64_t;

class GameObject
{
public:
    GameObjectID id = 0;
    std::string name = "GameObject";
    bool active = true;
    bool pendingDestroy = false;

    Transform2D transform;
    SpriteRenderer sprite;
    
    std::optional<SpriteAnimation> animation;
};
