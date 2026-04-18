#include "TileSetAsset.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace
{
    std::string ResolveProjectRelativePath(const std::filesystem::path& path)
    {
        const std::filesystem::path projectRoot = std::filesystem::current_path();

        std::error_code errorCode;
        const std::filesystem::path relativePath =
            std::filesystem::relative(path, projectRoot, errorCode);

        if (errorCode)
            return path.generic_string();

        return relativePath.generic_string();
    }

    std::string ResolveAssetRelativeTexturePath(
        const std::string& texturePath,
        const std::filesystem::path& assetPath)
    {
        if (texturePath.empty())
            return {};

        std::filesystem::path texture = std::filesystem::path(texturePath);
        if (texture.is_absolute() && std::filesystem::exists(texture))
            return ResolveProjectRelativePath(texture);

        if (std::filesystem::exists(texture))
            return texture.generic_string();

        const std::filesystem::path assetRelativePath = assetPath.parent_path() / texture;
        if (std::filesystem::exists(assetRelativePath))
            return ResolveProjectRelativePath(assetRelativePath);

        return texture.generic_string();
    }
}

void TileSetAsset::ResizeTileDefinitions()
{
    const size_t tileCount = static_cast<size_t>(m_Columns) * static_cast<size_t>(m_Rows);
    m_Tiles.resize(tileCount);
}

void TileSetAsset::SetGrid(uint32_t columns, uint32_t rows)
{
    m_Columns = std::max<uint32_t>(1, columns);
    m_Rows = std::max<uint32_t>(1, rows);
    ResizeTileDefinitions();
}

bool TileSetAsset::IsTileSolid(uint32_t tileID) const
{
    if (tileID >= m_Tiles.size())
        return false;

    return m_Tiles[tileID].solid;
}

void TileSetAsset::SetTileSolid(uint32_t tileID, bool solid)
{
    if (tileID >= m_Tiles.size())
        return;

    m_Tiles[tileID].solid = solid;
}

bool TileSetAsset::SaveToFile(const std::string& path) const
{
    json tiles = json::array();
    for (const TileDefinition& tile : m_Tiles)
    {
        tiles.push_back({
            {"solid", tile.solid}
        });
    }

    json root = {
        {"version", 1},
        {"assetType", "TileSet"},
        {"texturePath", m_TexturePath},
        {"columns", m_Columns},
        {"rows", m_Rows},
        {"tiles", tiles}
    };

    const std::filesystem::path filePath(path);
    if (filePath.has_parent_path())
        std::filesystem::create_directories(filePath.parent_path());

    std::ofstream out(filePath);
    if (!out.is_open())
        return false;

    out << root.dump(4);
    return true;
}

bool TileSetAsset::LoadFromFile(const std::string& path)
{
    try
    {
        const std::filesystem::path assetPath(path);

        std::ifstream in(assetPath);
        if (!in.is_open())
            return false;

        json root;
        in >> root;

        if (!root.contains("version") || root.at("version").get<int>() != 1)
            return false;

        if (root.contains("assetType") && root.at("assetType").get<std::string>() != "TileSet")
            return false;

        m_TexturePath = ResolveAssetRelativeTexturePath(
            root.value("texturePath", ""),
            assetPath
        );
        m_Columns = std::max<uint32_t>(1, root.value("columns", 1u));
        m_Rows = std::max<uint32_t>(1, root.value("rows", 1u));
        ResizeTileDefinitions();

        if (root.contains("tiles") && root.at("tiles").is_array())
        {
            const json& tiles = root.at("tiles");
            const size_t count = std::min(m_Tiles.size(), tiles.size());

            for (size_t i = 0; i < count; ++i)
                m_Tiles[i].solid = tiles[i].value("solid", false);
        }

        return true;
    }
    catch (const json::exception&)
    {
        return false;
    }
    catch (const std::exception&)
    {
        return false;
    }
}
