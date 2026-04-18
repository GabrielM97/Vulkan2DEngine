#pragma once

#include <array>
#include <string>

#include <glm/vec2.hpp>

#include "Scene/Scene.h"
class VulkanRenderer;

class TileMapEditorPanel
{
public:
    void Draw(Scene& scene, VulkanRenderer& renderer, GameObjectID selectedObjectID);
    void SyncFromSelection(Scene& scene, GameObjectID selectedObjectID);
    bool IsPaintModeEnabled() const { return m_PaintModeEnabled; }
    bool IsEraseModeEnabled() const { return m_EraseModeEnabled; }
    bool IsFillModeEnabled() const { return m_FillModeEnabled; }
    bool IsTileMapViewEnabled() const { return m_TileMapViewEnabled; }
    void SetTileMapViewEnabled(bool enabled) { m_TileMapViewEnabled = enabled; }
    int GetSelectedTileID() const { return m_SelectedTileID; }
    glm::ivec2 GetSelectedAtlasOrigin() const;
    glm::ivec2 GetSelectedAtlasSize() const;
    int GetSelectedTileIDAtOffset(glm::ivec2 offset, int atlasColumns) const;
    GameObjectID GetSelectedObjectID() const { return m_SelectedObjectID; }
    bool HasActiveTileMap(Scene& scene) const;
    glm::ivec2 GetHoveredTile() const { return m_HoveredTile; }
    void SetHoveredTile(glm::ivec2 tile) { m_HoveredTile = tile; }

private:
    void DrawAtlasPicker(Entity& entity, VulkanRenderer& renderer);
    void ApplyAtlusCellSize(Entity entity);
    bool ApplyTileSetAsset(Entity entity);
    void DrawLayerControls(Entity entity);
    void DrawAssetControls(Scene& scene, Entity entity);

    GameObjectID m_SelectedObjectID = 0;

    std::array<char, 260> m_TileMapTexturePathBuffer{};
    std::array<char, 260> m_TileSetAssetPathBuffer{};
    std::array<char, 260> m_TileMapAssetPathBuffer{};
    int m_TileMapWidthDraft = 16;
    int m_TileMapHeightDraft = 16;
    int m_TileMapColumnsDraft = 1;
    int m_TileMapRowsDraft = 1;
    int m_AtlasCellWidthDraft = 32;
    int m_AtlasCellHeightDraft = 32;
    int m_ActiveLayerDraft = 0;
    int m_SelectedTileID = 0;
    int m_TileEditX = 0;
    int m_TileEditY = 0;
    int m_TileEditValue = 0;
    bool m_PaintModeEnabled = true;
    bool m_EraseModeEnabled = false;
    bool m_FillModeEnabled = false;
    bool m_TileMapViewEnabled = false;
    glm::ivec2 m_HoveredTile{-1, -1};
    float m_AtlasPreviewScale = 1.0f;
    bool m_IsAtlasSelecting = false;
    glm::ivec2 m_AtlasSelectionStart{0, 0};
    glm::ivec2 m_AtlasSelectionEnd{0, 0};
};
