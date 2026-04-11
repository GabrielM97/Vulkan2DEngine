#pragma once

#include "Components.h"
#include "SpriteRenderer.h"

#include <string>

class GameObject
{
public:
    std::string name = "GameObject";
    bool active = true;
    bool pendingDestroy = false;

    Transform2D transform;
    SpriteRenderer sprite;
};
