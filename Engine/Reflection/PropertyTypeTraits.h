#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#include "Property.h"

template<typename T>
struct PropertyTypeTraits;

template<>
struct PropertyTypeTraits<float>
{
    static constexpr PropertyType Type = PropertyType::Float;
};

template<>
struct PropertyTypeTraits<bool>
{
    static constexpr PropertyType Type = PropertyType::Bool;
};

template<>
struct PropertyTypeTraits<int>
{
    static constexpr PropertyType Type = PropertyType::Int;
};

template<>
struct PropertyTypeTraits<uint32_t>
{
    static constexpr PropertyType Type = PropertyType::UInt;
};

template<>
struct PropertyTypeTraits<std::string>
{
    static constexpr PropertyType Type = PropertyType::String;
};

template<>
struct PropertyTypeTraits<glm::vec2>
{
    static constexpr PropertyType Type = PropertyType::Vec2;
};

template<>
struct PropertyTypeTraits<glm::vec4>
{
    static constexpr PropertyType Type = PropertyType::Vec4;
};