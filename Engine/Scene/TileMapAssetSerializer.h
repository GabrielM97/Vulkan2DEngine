#pragma once

#include <string>

class Entity;

class TileMapAssetSerializer
{
public:
    static bool SaveToFile(Entity entity, const std::string& path);
    static bool LoadIntoEntity(const std::string& path, Entity entity);
};
