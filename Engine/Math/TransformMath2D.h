#pragma once

#include "Transform2D.h"

#include <glm/vec2.hpp>

namespace TransformMath2D
{
    glm::vec2 RotateVector(const glm::vec2& value, float degrees);
    glm::vec2 SafeDivide(const glm::vec2& value, const glm::vec2& divisor);

    glm::vec2 GetPivotLocalPosition(const Transform2D& transform, const glm::vec2& size);
    glm::vec2 GetPivotWorldPosition(const Transform2D& transform, const glm::vec2& size);

    Transform2D CombineTransforms(
        const Transform2D& parentWorld,
        const glm::vec2& parentSize,
        const Transform2D& childLocal,
        const glm::vec2& childSize
    );

    Transform2D ToLocalTransform(
        const Transform2D& worldTransform,
        const glm::vec2& worldSize,
        const Transform2D& parentWorldTransform,
        const glm::vec2& parentSize
    );
}
