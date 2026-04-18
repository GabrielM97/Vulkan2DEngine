#pragma once

#include <array>

#include "Scene/TileSetAsset.h"

class VulkanRenderer;

class TileSetEditorPanel
{
public:
    void Draw(VulkanRenderer& renderer);

private:
    void SyncBuffersFromAsset();
    void ClampSelectedTile();
    void ApplyCellSizeFromTexture();
    void DrawAtlasPicker(VulkanRenderer& renderer);

    TileSetAsset m_TileSetAsset;
    std::array<char, 260> m_AssetPathBuffer{};
    std::array<char, 260> m_TexturePathBuffer{};
    int m_CellWidthDraft = 32;
    int m_CellHeightDraft = 32;
    int m_ColumnsDraft = 1;
    int m_RowsDraft = 1;
    int m_SelectedTileID = 0;
    float m_PreviewScale = 1.0f;
};
