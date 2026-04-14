#pragma once

#include <cstdint>

using ComponentTypeID = uint32_t;

class EntityComponent
{
public:
    virtual ~EntityComponent() = default;

    virtual ComponentTypeID GetID() const = 0;
    virtual const char* GetTypeName() const = 0;
};