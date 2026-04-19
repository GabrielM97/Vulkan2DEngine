#pragma once

#include <cstdint>

#include <glm/vec2.hpp>

struct AABB2D
{
    glm::vec2 min{0.0f, 0.0f};
    glm::vec2 max{0.0f, 0.0f};
};

struct OverlapResult
{
    enum class Type
    {
        Entity,
        Tile
    };

    Type type = Type::Entity;
    uint64_t objectID = 0;
    uint32_t tileLayerIndex = 0;
    glm::ivec2 tileCoordinates{-1, -1};
    glm::vec2 HitPosition{0.0f, 0.0f};

    bool IsEntity() const { return type == Type::Entity; }
    bool IsTile() const { return type == Type::Tile; }
};
