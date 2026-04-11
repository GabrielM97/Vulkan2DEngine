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

struct IntRect
{
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

struct SpriteRenderer
{
    TextureRef texture;
    IntRect sourceRect{0, 0, 0, 0};
    bool useSourceRect = false;
    glm::vec4 tint{1.0f};
    bool visible = true;
    int layer = 0;
};

inline void ClearSourceRect(SpriteRenderer& sprite)
{
    sprite.sourceRect = {0, 0, 0, 0};
    sprite.useSourceRect = false;
}

inline void SetSourceRect(SpriteRenderer& sprite, int x, int y, int width, int height)
{
    sprite.sourceRect = {x, y, width, height};
    sprite.useSourceRect = true;
}

inline void SetSourceRectFromGrid(
    SpriteRenderer& sprite,
    int column,
    int row,
    int cellWidth,
    int cellHeight)
{
    SetSourceRect(
        sprite,
        column * cellWidth,
        row * cellHeight,
        cellWidth,
        cellHeight
    );
}
