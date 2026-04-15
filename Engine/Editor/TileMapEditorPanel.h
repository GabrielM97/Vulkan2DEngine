#pragma once

#include <array>

#include <glm/vec2.hpp>

#include "Scene/Scene.h"
class TileMapEditorPanel
{
public:
    void Draw(Scene& scene, GameObjectID selectedObjectID);
    void SyncFromSelection(Scene& scene, GameObjectID selectedObjectID);
    bool IsPaintModeEnabled() const { return m_PaintModeEnabled; }
    bool IsEraseModeEnabled() const { return m_EraseModeEnabled; }
    int GetSelectedTileID() const { return m_SelectedTileID; }
    GameObjectID GetSelectedObjectID() const { return m_SelectedObjectID; }
    bool HasActiveTileMap(Scene& scene) const;
    glm::ivec2 GetHoveredTile() const { return m_HoveredTile; }
    void SetHoveredTile(glm::ivec2 tile) { m_HoveredTile = tile; }

private:
    GameObjectID m_SelectedObjectID = 0;

    std::array<char, 260> m_TileMapTexturePathBuffer{};
    int m_TileMapWidthDraft = 16;
    int m_TileMapHeightDraft = 16;
    int m_TileMapColumnsDraft = 1;
    int m_TileMapRowsDraft = 1;
    int m_SelectedTileID = 0;
    int m_TileEditX = 0;
    int m_TileEditY = 0;
    int m_TileEditValue = 0;
    bool m_PaintModeEnabled = true;
    bool m_EraseModeEnabled = false;
    glm::ivec2 m_HoveredTile{-1, -1};
};
