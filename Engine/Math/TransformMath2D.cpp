#include "TransformMath2D.h"

#include <cmath>
#include <glm/trigonometric.hpp>

namespace TransformMath2D
{
    glm::vec2 RotateVector(const glm::vec2& value, float degrees)
    {
        const float radians = glm::radians(degrees);
        const float c = std::cos(radians);
        const float s = std::sin(radians);

        return {
            value.x * c - value.y * s,
            value.x * s + value.y * c
        };
    }

    glm::vec2 SafeDivide(const glm::vec2& value, const glm::vec2& divisor)
    {
        glm::vec2 result = value;

        if (divisor.x != 0.0f)
            result.x /= divisor.x;
        if (divisor.y != 0.0f)
            result.y /= divisor.y;

        return result;
    }

    glm::vec2 GetPivotLocalPosition(const Transform2D& transform, const glm::vec2& size)
    {
        return transform.position + transform.pivot * (size * transform.scale);
    }

    glm::vec2 GetPivotWorldPosition(const Transform2D& transform, const glm::vec2& size)
    {
        return GetPivotLocalPosition(transform, size);
    }

    Transform2D CombineTransforms(
        const Transform2D& parentWorld,
        const glm::vec2& parentSize,
        const Transform2D& childLocal,
        const glm::vec2& childSize)
    {
        Transform2D combined = childLocal;

        const glm::vec2 parentPivotOffset = parentWorld.pivot * (parentSize * parentWorld.scale);
        const glm::vec2 childPivotLocal = childLocal.position + childLocal.pivot * (childSize * childLocal.scale);

        const glm::vec2 rotatedOffset = RotateVector(
            childPivotLocal * parentWorld.scale - parentPivotOffset,
            parentWorld.rotationDegrees
        );

        const glm::vec2 parentPivotWorld = GetPivotWorldPosition(parentWorld, parentSize);
        const glm::vec2 childPivotWorld = parentPivotWorld + rotatedOffset;

        combined.rotationDegrees = parentWorld.rotationDegrees + childLocal.rotationDegrees;
        combined.scale = parentWorld.scale * childLocal.scale;
        combined.pivot = childLocal.pivot;
        combined.position = childPivotWorld - combined.pivot * (childSize * combined.scale);

        return combined;
    }

    Transform2D ToLocalTransform(
        const Transform2D& worldTransform,
        const glm::vec2& worldSize,
        const Transform2D& parentWorldTransform,
        const glm::vec2& parentSize)
    {
        Transform2D local = worldTransform;

        const glm::vec2 worldPivot = GetPivotWorldPosition(worldTransform, worldSize);
        const glm::vec2 parentPivotWorld = GetPivotWorldPosition(parentWorldTransform, parentSize);
        const glm::vec2 parentPivotOffset = parentWorldTransform.pivot * (parentSize * parentWorldTransform.scale);

        glm::vec2 localPivotOffset = worldPivot - parentPivotWorld;
        localPivotOffset = RotateVector(localPivotOffset, -parentWorldTransform.rotationDegrees);
        localPivotOffset = SafeDivide(localPivotOffset, parentWorldTransform.scale);

        const glm::vec2 localPivot = parentPivotOffset + localPivotOffset;
        local.rotationDegrees = worldTransform.rotationDegrees - parentWorldTransform.rotationDegrees;
        local.scale = SafeDivide(worldTransform.scale, parentWorldTransform.scale);
        local.pivot = worldTransform.pivot;
        local.position = localPivot - local.pivot * (worldSize * local.scale);

        return local;
    }
}
