#pragma once

#include <cstdint>
#include <string_view>
#include <span>
#include <array>
#include <cstddef>

enum class PropertyType
{  
    Float,
   Bool,
   Int,
   UInt,
   String,
   Vec2,
   Vec4
};

enum PropertyFlags : uint32_t
{
    PropertyNone = 0,
    EditAnywhere = 1 << 0,
    Save = 1 << 1
};

struct Property
{
    std::string_view name;
    std::size_t offset = 0;
    std::size_t size = 0;
    uint32_t flags = PropertyNone;
    PropertyType type = PropertyType::Float;
};

template<typename T>
struct PropertyRegistry
{
    static constexpr std::span<const Property> Get()
    {
        return {};
    }
};

#define PROPERTY_FIELD(Type, Name, Flags, ...) \
Type Name = __VA_ARGS__

#define PROPERTY_INFO(OwnerType, Name, Flags) \
Property{ \
#Name, \
offsetof(OwnerType, Name), \
sizeof(decltype(OwnerType::Name)), \
Flags, \
PropertyTypeTraits<decltype(OwnerType::Name)>::Type \
}

#define REGISTER_PROPERTIES(Type, ...) \
template<> \
struct PropertyRegistry<Type> \
{ \
static constexpr auto Properties = std::to_array<Property>({ __VA_ARGS__ }); \
static constexpr std::span<const Property> Get() \
{ \
return Properties; \
} \
}
