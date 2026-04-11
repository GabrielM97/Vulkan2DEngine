#pragma once
#include <cstdint>
#include <string>
#include <glm/glm.hpp>

struct Transform2D
{
    glm::vec2 position{0.0f};
    glm::vec2 size{1.0f};
    float rotationDegrees = 0.0f;
};

struct TextureRef
{
    std::string path;
};

struct SpriteRenderer
{
    TextureRef texture;
    glm::vec4 tint{1.0f};
    bool visible = true;
    int layer = 0;
};