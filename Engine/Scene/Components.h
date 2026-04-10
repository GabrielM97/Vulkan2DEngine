#pragma once
#include <cstdint>
#include <glm/glm.hpp>

struct Transform2D
{
    glm::vec2 position{0.0f};
    glm::vec2 size{1.0f};
    float rotationDegrees = 0.0f;
};

struct SpriteRenderer
{
    uint32_t textureIndex = 0;
    glm::vec4 tint{1.0f};

    // Higher layers are rendered later, so they appear in front.
    int layer = 0;
};
