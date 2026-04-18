#include "TileMapAssetSerializer.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>

#include <nlohmann/json.hpp>

#include "Entity.h"

using json = nlohmann::json;

namespace
{
    json SerializeVec2(const glm::vec2& value)
    {
        return json::array({value.x, value.y});
    }

    glm::vec2 DeserializeVec2(const json& value)
    {
        return {
            value.at(0).get<float>(),
            value.at(1).get<float>()
        };
    }

    json SerializeTileMap(const TileMapComponent& tileMap)
    {
        json layers = json::array();
        for (const auto& layer : tileMap.layers)
        {
            layers.push_back({
                {"name", layer.name},
                {"visible", layer.visible},
                {"collisionEnabled", layer.collisionEnabled},
                {"tiles", layer.tiles}
            });
        }

        return {
            {"width", tileMap.width},
            {"height", tileMap.height},
            {"tileSize", SerializeVec2(tileMap.tileSize)},
            {"atlasCellSize", json::array({tileMap.atlasCellSize.x, tileMap.atlasCellSize.y})},
            {"columns", tileMap.columns},
            {"rows", tileMap.rows},
            {"tilesetTexturePath", tileMap.tilesetTexturePath},
            {"tileSetAssetPath", tileMap.tileSetAssetPath},
            {"assetPath", tileMap.assetPath},
            {"activeLayerIndex", tileMap.activeLayerIndex},
            {"layers", layers}
        };
    }

    bool DeserializeTileMap(const json& value, TileMapComponent& tileMap)
    {
        tileMap.width = value.value("width", 0u);
        tileMap.height = value.value("height", 0u);
        tileMap.tileSize = value.contains("tileSize")
            ? DeserializeVec2(value.at("tileSize"))
            : glm::vec2{32.0f, 32.0f};

        if (value.contains("atlasCellSize") && value.at("atlasCellSize").is_array() && value.at("atlasCellSize").size() >= 2)
        {
            const json& atlasCellSize = value.at("atlasCellSize");
            tileMap.atlasCellSize = {
                std::max(1, atlasCellSize.at(0).get<int>()),
                std::max(1, atlasCellSize.at(1).get<int>())
            };
        }
        else
        {
            tileMap.atlasCellSize = {
                std::max(1, static_cast<int>(tileMap.tileSize.x)),
                std::max(1, static_cast<int>(tileMap.tileSize.y))
            };
        }

        tileMap.columns = std::max<uint32_t>(1, value.value("columns", 1u));
        tileMap.rows = std::max<uint32_t>(1, value.value("rows", 1u));
        tileMap.tilesetTexturePath = value.value("tilesetTexturePath", "");
        tileMap.tileSetAssetPath = value.value("tileSetAssetPath", "");
        tileMap.assetPath = value.value("assetPath", "");

        const std::size_t expectedCount =
            static_cast<std::size_t>(tileMap.width) * static_cast<std::size_t>(tileMap.height);

        tileMap.layers.clear();

        if (value.contains("layers") && value.at("layers").is_array())
        {
            for (const json& layerValue : value.at("layers"))
            {
                TileMapComponent::Layer layer;
                layer.name = layerValue.value("name", "Layer");
                layer.visible = layerValue.value("visible", true);
                layer.collisionEnabled = layerValue.value("collisionEnabled", false);
                layer.tiles = layerValue.value("tiles", std::vector<int32_t>{});

                if (layer.tiles.size() != expectedCount)
                    layer.tiles.assign(expectedCount, -1);

                tileMap.layers.push_back(std::move(layer));
            }
        }
        else
        {
            TileMapComponent::Layer layer;
            layer.name = "Layer 0";
            layer.visible = true;
            layer.tiles = value.value("tiles", std::vector<int32_t>{});
            if (layer.tiles.size() != expectedCount)
                layer.tiles.assign(expectedCount, -1);
            tileMap.layers.push_back(std::move(layer));
        }

        if (tileMap.layers.empty())
        {
            TileMapComponent::Layer layer;
            layer.name = "Layer 0";
            layer.visible = true;
            layer.tiles.assign(expectedCount, -1);
            tileMap.layers.push_back(std::move(layer));
        }

        tileMap.activeLayerIndex = std::min<uint32_t>(
            value.value("activeLayerIndex", 0u),
            static_cast<uint32_t>(tileMap.layers.size() - 1)
        );

        return true;
    }
}

bool TileMapAssetSerializer::SaveToFile(Entity entity, const std::string& path)
{
    if (!entity.IsValid() || !entity.HasTileMap())
        return false;

    const std::filesystem::path filePath(path);
    if (filePath.has_parent_path())
        std::filesystem::create_directories(filePath.parent_path());

    std::ofstream out(filePath);
    if (!out.is_open())
        return false;

    json root;
    root["version"] = 1;
    root["assetType"] = "TileMap";
    TileMapComponent tileMap = entity.GetTileMapData();
    tileMap.assetPath = path;
    root["tileMap"] = SerializeTileMap(tileMap);

    out << root.dump(4);
    return true;
}

bool TileMapAssetSerializer::LoadIntoEntity(const std::string& path, Entity entity)
{
    if (!entity.IsValid())
        return false;

    std::ifstream in(path);
    if (!in.is_open())
        return false;

    json root;
    in >> root;

    if (!root.contains("version") || root.at("version").get<int>() != 1)
        return false;

    if (root.contains("assetType") && root.at("assetType").get<std::string>() != "TileMap")
        return false;

    if (!root.contains("tileMap") || !root.at("tileMap").is_object())
        return false;

    TileMapComponent tileMap;
    if (!DeserializeTileMap(root.at("tileMap"), tileMap))
        return false;

    tileMap.assetPath = path;
    entity.SetTileMapData(tileMap);
    return true;
}
