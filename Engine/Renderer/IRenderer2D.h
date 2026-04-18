#pragma once

#include "Scene/Camera2D.h"
#include "Scene/SpriteRenderer.h"

struct Transform2D;

class IRenderer2D
{
public:
    virtual ~IRenderer2D() = default;
    virtual void SetCamera(const Camera2D& camera) = 0;

    virtual void DrawSprite(
        const Transform2D& transform,
        const SpriteRenderer& sprite
    ) = 0;
    
    virtual void DrawRectOutline(
        const glm::vec2& min,
        const glm::vec2& max,
        const glm::vec4& color,
        float thickness = 2.0f
    ) = 0;
};
