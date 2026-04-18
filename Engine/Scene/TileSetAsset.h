#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct TileDefinition
{
    bool solid = false;
};

class TileSetAsset
{
public:
    bool LoadFromFile(const std::string& path);
    bool SaveToFile(const std::string& path) const;

    const std::string& GetTexturePath() const { return m_TexturePath; }
    void SetTexturePath(const std::string& path) { m_TexturePath = path; }

    uint32_t GetColumns() const { return m_Columns; }
    uint32_t GetRows() const { return m_Rows; }
    void SetGrid(uint32_t columns, uint32_t rows);

    bool IsTileSolid(uint32_t tileID) const;
    void SetTileSolid(uint32_t tileID, bool solid);

    uint32_t GetTileCount() const { return static_cast<uint32_t>(m_Tiles.size()); }

private:
    void ResizeTileDefinitions();

    std::string m_TexturePath;
    uint32_t m_Columns = 1;
    uint32_t m_Rows = 1;
    std::vector<TileDefinition> m_Tiles;
};
