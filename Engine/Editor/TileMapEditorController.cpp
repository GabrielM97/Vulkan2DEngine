#include "Editor/TileMapEditorController.h"

#include <cmath>
#include <queue>

#include <GLFW/glfw3.h>

#include "Editor/EditorViewportMath.h"
#include "Editor/TileMapEditorPanel.h"

bool TileMapEditorController::TryGetHoveredTile(
    Scene& scene,
    const SceneViewportState& viewportState,
    const InputState& input,
    Entity entity,
    glm::ivec2& outTile) const
{
    if (!entity.IsValid() || !entity.HasTileMap())
        return false;

    const glm::vec2 mouseScreen = input.GetMouseScreenPosition();
    const glm::vec2 worldPosition = EditorViewportMath::ScreenToWorld(scene, mouseScreen, viewportState);
    const Transform2D mapTransform = entity.GetTransform();
    const glm::vec2 tileSize = entity.GetTileSize();

    if (tileSize.x <= 0.0f || tileSize.y <= 0.0f)
        return false;

    const glm::vec2 local = worldPosition - mapTransform.position;
    const int tileX = static_cast<int>(std::floor(local.x / tileSize.x));
    const int tileY = static_cast<int>(std::floor(local.y / tileSize.y));

    if (tileX < 0 || tileY < 0)
        return false;

    if (tileX >= static_cast<int>(entity.GetTileMapWidth()) ||
        tileY >= static_cast<int>(entity.GetTileMapHeight()))
    {
        return false;
    }

    outTile = {tileX, tileY};
    return true;
}

void TileMapEditorController::BeginTileStroke(Entity entity)
{
    m_TileStrokeActive = true;
    m_ActiveTileStroke = {};
    m_ActiveTileStroke.targetID = entity.GetID();
}

void TileMapEditorController::RecordTileStrokeEdit(
    Entity entity,
    uint32_t layerIndex,
    int tileX,
    int tileY,
    int32_t newValue)
{
    if (!m_TileStrokeActive || !entity.IsValid() || !entity.HasTileMap())
        return;

    const int32_t oldValue = entity.GetTile(layerIndex, tileX, tileY);
    if (oldValue == newValue)
        return;

    const int64_t key =
        (static_cast<int64_t>(tileY) << 32) |
        static_cast<uint32_t>(tileX);

    auto lookupIt = m_ActiveTileStroke.cellLookup.find(key);
    if (lookupIt == m_ActiveTileStroke.cellLookup.end())
    {
        m_ActiveTileStroke.cellLookup.emplace(key, m_ActiveTileStroke.cells.size());
        m_ActiveTileStroke.cells.push_back({
            layerIndex,
            tileX,
            tileY,
            oldValue,
            newValue
        });
    }
    else
    {
        m_ActiveTileStroke.cells[lookupIt->second].after = newValue;
    }

    entity.SetTile(layerIndex, tileX, tileY, newValue);
}

void TileMapEditorController::ApplyTileStrokeStamp(
    TileMapEditorPanel& tileMapPanel,
    Entity entity,
    glm::ivec2 hoveredTile,
    bool erase)
{
    if (!m_TileStrokeActive || !entity.IsValid() || !entity.HasTileMap())
        return;

    const glm::ivec2 selectionSize = tileMapPanel.GetSelectedAtlasSize();
    const int atlasColumns = static_cast<int>(entity.GetTileMapColumns());
    const uint32_t activeLayer = entity.GetActiveTileLayerIndex();

    for (int offsetY = 0; offsetY < selectionSize.y; ++offsetY)
    {
        for (int offsetX = 0; offsetX < selectionSize.x; ++offsetX)
        {
            const int tileX = hoveredTile.x + offsetX;
            const int tileY = hoveredTile.y + offsetY;
            const int32_t newValue = erase
                ? -1
                : tileMapPanel.GetSelectedTileIDAtOffset({offsetX, offsetY}, atlasColumns);
            RecordTileStrokeEdit(entity, activeLayer, tileX, tileY, newValue);
        }
    }
}

void TileMapEditorController::ApplyTileStrokeFill(
    TileMapEditorPanel& tileMapPanel,
    Entity entity,
    glm::ivec2 hoveredTile,
    bool erase)
{
    if (!m_TileStrokeActive || !entity.IsValid() || !entity.HasTileMap())
        return;

    const glm::ivec2 selectionSize = tileMapPanel.GetSelectedAtlasSize();
    const int atlasColumns = static_cast<int>(entity.GetTileMapColumns());
    const uint32_t activeLayer = entity.GetActiveTileLayerIndex();
    const uint32_t mapWidth = entity.GetTileMapWidth();
    const uint32_t mapHeight = entity.GetTileMapHeight();

    if (hoveredTile.x < 0 || hoveredTile.y < 0 ||
        hoveredTile.x >= static_cast<int>(mapWidth) ||
        hoveredTile.y >= static_cast<int>(mapHeight))
    {
        return;
    }

    const int32_t targetValue = entity.GetTile(activeLayer, hoveredTile.x, hoveredTile.y);
    const auto computeFillValue = [&](int tileX, int tileY) -> int32_t
    {
        if (erase)
            return -1;

        const int offsetX = (tileX - hoveredTile.x) % std::max(1, selectionSize.x);
        const int offsetY = (tileY - hoveredTile.y) % std::max(1, selectionSize.y);
        const int wrappedOffsetX = offsetX < 0 ? offsetX + selectionSize.x : offsetX;
        const int wrappedOffsetY = offsetY < 0 ? offsetY + selectionSize.y : offsetY;
        return tileMapPanel.GetSelectedTileIDAtOffset({wrappedOffsetX, wrappedOffsetY}, atlasColumns);
    };

    const int32_t hoveredNewValue = computeFillValue(hoveredTile.x, hoveredTile.y);
    if (targetValue == hoveredNewValue)
        return;

    std::vector<uint8_t> visited(static_cast<size_t>(mapWidth) * static_cast<size_t>(mapHeight), 0);
    std::queue<glm::ivec2> queue;
    queue.push(hoveredTile);

    while (!queue.empty())
    {
        const glm::ivec2 current = queue.front();
        queue.pop();

        if (current.x < 0 || current.y < 0 ||
            current.x >= static_cast<int>(mapWidth) ||
            current.y >= static_cast<int>(mapHeight))
        {
            continue;
        }

        const size_t visitedIndex =
            static_cast<size_t>(current.y) * static_cast<size_t>(mapWidth) +
            static_cast<size_t>(current.x);
        if (visited[visitedIndex] != 0)
            continue;

        visited[visitedIndex] = 1;

        if (entity.GetTile(activeLayer, current.x, current.y) != targetValue)
            continue;

        RecordTileStrokeEdit(entity, activeLayer, current.x, current.y, computeFillValue(current.x, current.y));

        queue.push({current.x + 1, current.y});
        queue.push({current.x - 1, current.y});
        queue.push({current.x, current.y + 1});
        queue.push({current.x, current.y - 1});
    }
}

void TileMapEditorController::EndTileStroke()
{
    if (!m_TileStrokeActive)
        return;

    m_TileStrokeActive = false;

    if (!m_ActiveTileStroke.cells.empty())
    {
        m_ActiveTileStroke.cellLookup.clear();
        m_UndoTileStrokes.push_back(std::move(m_ActiveTileStroke));
        m_RedoTileStrokes.clear();
    }
    else
    {
        m_ActiveTileStroke = {};
    }
}

void TileMapEditorController::UndoTileStroke(Scene& scene)
{
    if (m_UndoTileStrokes.empty())
        return;

    TileEditStroke stroke = std::move(m_UndoTileStrokes.back());
    m_UndoTileStrokes.pop_back();

    Entity entity = scene.GetEntity(stroke.targetID);
    if (entity.IsValid() && entity.HasTileMap())
    {
        for (const TileEditCell& cell : stroke.cells)
            entity.SetTile(cell.layerIndex, cell.x, cell.y, cell.before);
    }

    stroke.cellLookup.clear();
    m_RedoTileStrokes.push_back(std::move(stroke));
}

void TileMapEditorController::RedoTileStroke(Scene& scene)
{
    if (m_RedoTileStrokes.empty())
        return;

    TileEditStroke stroke = std::move(m_RedoTileStrokes.back());
    m_RedoTileStrokes.pop_back();

    Entity entity = scene.GetEntity(stroke.targetID);
    if (entity.IsValid() && entity.HasTileMap())
    {
        for (const TileEditCell& cell : stroke.cells)
            entity.SetTile(cell.layerIndex, cell.x, cell.y, cell.after);
    }

    stroke.cellLookup.clear();
    m_UndoTileStrokes.push_back(std::move(stroke));
}

bool TileMapEditorController::IsUndoShortcutPressed(const InputState& input) const
{
    const bool controlDown =
        input.IsKeyDown(GLFW_KEY_LEFT_CONTROL) ||
        input.IsKeyDown(GLFW_KEY_RIGHT_CONTROL);

    return controlDown && input.WasKeyPressed(GLFW_KEY_Z);
}

bool TileMapEditorController::IsRedoShortcutPressed(const InputState& input) const
{
    const bool controlDown =
        input.IsKeyDown(GLFW_KEY_LEFT_CONTROL) ||
        input.IsKeyDown(GLFW_KEY_RIGHT_CONTROL);

    return controlDown && input.WasKeyPressed(GLFW_KEY_Y);
}

void TileMapEditorController::Update(
    Scene& scene,
    TileMapEditorPanel& tileMapPanel,
    const InputState& input,
    const SceneViewportState& viewportState,
    bool allowEditing)
{
    if (allowEditing && input.CanUseEditorShortcuts() && IsUndoShortcutPressed(input))
        UndoTileStroke(scene);

    if (allowEditing && input.CanUseEditorShortcuts() && IsRedoShortcutPressed(input))
        RedoTileStroke(scene);

    if (allowEditing && viewportState.visible && viewportState.hovered && tileMapPanel.IsTileMapViewEnabled())
    {
        Entity selected = scene.GetEntity(tileMapPanel.GetSelectedObjectID());

        glm::ivec2 hoveredTile{-1, -1};
        if (tileMapPanel.HasActiveTileMap(scene) && TryGetHoveredTile(scene, viewportState, input, selected, hoveredTile))
        {
            tileMapPanel.SetHoveredTile(hoveredTile);

            const bool fillStroke =
                input.CanUseEditorViewportInput() &&
                input.WasMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) &&
                tileMapPanel.IsFillModeEnabled();

            const bool paintStroke =
                input.CanUseEditorViewportInput() &&
                input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) &&
                tileMapPanel.IsPaintModeEnabled() &&
                !tileMapPanel.IsFillModeEnabled();

            const bool eraseStroke =
                input.CanUseEditorViewportInput() &&
                (input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT) ||
                 (tileMapPanel.IsEraseModeEnabled() && input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT))) &&
                !tileMapPanel.IsFillModeEnabled();

            if (fillStroke)
            {
                BeginTileStroke(selected);
                ApplyTileStrokeFill(tileMapPanel, selected, hoveredTile, tileMapPanel.IsEraseModeEnabled());
                EndTileStroke();
            }

            if ((paintStroke || eraseStroke) && !m_TileStrokeActive)
                BeginTileStroke(selected);

            if (paintStroke)
                ApplyTileStrokeStamp(tileMapPanel, selected, hoveredTile, false);

            if (eraseStroke)
                ApplyTileStrokeStamp(tileMapPanel, selected, hoveredTile, true);
        }
        else
        {
            tileMapPanel.SetHoveredTile(glm::ivec2{-1, -1});
        }
    }
    else
    {
        tileMapPanel.SetHoveredTile(glm::ivec2{-1, -1});
    }

    if (m_TileStrokeActive &&
        (input.WasMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT) ||
         input.WasMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT)))
    {
        EndTileStroke();
    }
}
