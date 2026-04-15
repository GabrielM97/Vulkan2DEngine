#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "Reflection/Property.h"

template<typename T>
void SerializeProperties(const T& object, nlohmann::json& out)
{
    const std::byte* base = reinterpret_cast<const std::byte*>(&object);

    for (const Property& property : PropertyRegistry<T>::Get())
    {
        if ((property.flags & Save) == 0)
            continue;

        const std::byte* fieldPtr = base + property.offset;

        switch (property.type)
        {
        case PropertyType::Float:
            out[property.name] = *reinterpret_cast<const float*>(fieldPtr);
            break;

        case PropertyType::Bool:
            out[property.name] = *reinterpret_cast<const bool*>(fieldPtr);
            break;

        case PropertyType::Int:
            out[property.name] = *reinterpret_cast<const int*>(fieldPtr);
            break;

        case PropertyType::UInt:
            out[property.name] = *reinterpret_cast<const uint32_t*>(fieldPtr);
            break;

        case PropertyType::String:
            out[property.name] = *reinterpret_cast<const std::string*>(fieldPtr);
            break;

        case PropertyType::Vec2:
            {
                const auto& value = *reinterpret_cast<const glm::vec2*>(fieldPtr);
                out[property.name] = nlohmann::json::array({value.x, value.y});
                break;
            }

        case PropertyType::Vec4:
            {
                const auto& value = *reinterpret_cast<const glm::vec4*>(fieldPtr);
                out[property.name] = nlohmann::json::array({value.x, value.y, value.z, value.w});
                break;
            }
        }
    }
}

template<typename T>
void DeserializeProperties(T& object, const nlohmann::json& in)
{
    std::byte* base = reinterpret_cast<std::byte*>(&object);

    for (const Property& property : PropertyRegistry<T>::Get())
    {
        if ((property.flags & Save) == 0)
            continue;

        if (!in.contains(property.name))
            continue;

        std::byte* fieldPtr = base + property.offset;

        switch (property.type)
        {
        case PropertyType::Float:
            *reinterpret_cast<float*>(fieldPtr) = in.at(property.name).get<float>();
            break;

        case PropertyType::Bool:
            *reinterpret_cast<bool*>(fieldPtr) = in.at(property.name).get<bool>();
            break;

        case PropertyType::Int:
            *reinterpret_cast<int*>(fieldPtr) = in.at(property.name).get<int>();
            break;

        case PropertyType::UInt:
            *reinterpret_cast<uint32_t*>(fieldPtr) = in.at(property.name).get<uint32_t>();
            break;

        case PropertyType::String:
            *reinterpret_cast<std::string*>(fieldPtr) = in.at(property.name).get<std::string>();
            break;

        case PropertyType::Vec2:
            {
                const auto& value = in.at(property.name);
                *reinterpret_cast<glm::vec2*>(fieldPtr) = {
                    value.at(0).get<float>(),
                    value.at(1).get<float>()
                };
                break;
            }

        case PropertyType::Vec4:
            {
                const auto& value = in.at(property.name);
                *reinterpret_cast<glm::vec4*>(fieldPtr) = {
                    value.at(0).get<float>(),
                    value.at(1).get<float>(),
                    value.at(2).get<float>(),
                    value.at(3).get<float>()
                };
                break;
            }
        }
    }
}
