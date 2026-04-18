#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Core/InputState.h"
#include "Editor/SceneViewportPanel.h"
#include "Scene/Scene.h"

class TileMapEditorPanel;

class TileMapEditorController
{
public:
    void Update(
        Scene& scene,
        TileMapEditorPanel& tileMapPanel,
        const InputState& input,
        const SceneViewportState& viewportState,
        bool allowEditing);

private:
    struct TileEditCell
    {
        uint32_t layerIndex = 0;
        int x = 0;
        int y = 0;
        int32_t before = -1;
        int32_t after = -1;
    };

    struct TileEditStroke
    {
        GameObjectID targetID = 0;
        std::vector<TileEditCell> cells;
        std::unordered_map<int64_t, size_t> cellLookup;
    };

    bool TryGetHoveredTile(
        Scene& scene,
        const SceneViewportState& viewportState,
        const InputState& input,
        Entity entity,
        glm::ivec2& outTile) const;
    void BeginTileStroke(Entity entity);
    void RecordTileStrokeEdit(Entity entity, uint32_t layerIndex, int tileX, int tileY, int32_t newValue);
    void ApplyTileStrokeStamp(TileMapEditorPanel& tileMapPanel, Entity entity, glm::ivec2 hoveredTile, bool erase);
    void ApplyTileStrokeFill(TileMapEditorPanel& tileMapPanel, Entity entity, glm::ivec2 hoveredTile, bool erase);
    void EndTileStroke();
    void UndoTileStroke(Scene& scene);
    void RedoTileStroke(Scene& scene);
    bool IsUndoShortcutPressed(const InputState& input) const;
    bool IsRedoShortcutPressed(const InputState& input) const;

    bool m_TileStrokeActive = false;
    TileEditStroke m_ActiveTileStroke;
    std::vector<TileEditStroke> m_UndoTileStrokes;
    std::vector<TileEditStroke> m_RedoTileStrokes;
};
