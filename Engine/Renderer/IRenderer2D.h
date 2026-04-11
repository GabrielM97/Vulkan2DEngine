#pragma once

#include "Scene/Components.h"

struct Transform2D;

class IRenderer2D
{
public:
    virtual ~IRenderer2D() = default;

    virtual void DrawSprite(
        const Transform2D& transform,
        const SpriteRenderer& sprite
    ) = 0;
};