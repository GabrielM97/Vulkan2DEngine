#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Core/Application.h"
#include "Object/Player.h"
#include "Scene/Scene.h"

class SandboxApp : public Application
{
protected:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(VulkanRenderer& renderer) override;
    Scene* GetEditorScene() override { return &m_Scene; }
    bool IsEditorPlaying() const override;
    void OnEditorPlay() override;
    void OnEditorStop() override;

private:
    enum class EditorMode
    {
        Editing,
        Playing
    };

    void CreateDefaultScene();
    void RefreshRuntimeHandles();
    void EnterPlayMode();
    void ExitPlayMode();
    bool TryGetHoveredTile(Entity entity, glm::ivec2& outTile) const;
    bool TryGetSelectionShape(Entity entity, Transform2D& outTransform, glm::vec2& outSize) const;
    bool TrySelectedGameObject(GameObjectID& outObjectID) const;
    glm::vec2 ScreenToWorld(const glm::vec2& screenPosition, const SceneViewportState& viewportState) const;
    void BeginTileStroke(Entity entity);
    void RecordTileStrokeEdit(Entity entity, uint32_t layerIndex, int tileX, int tileY, int32_t newValue);
    void ApplyTileStrokeStamp(Entity entity, glm::ivec2 hoveredTile, bool erase);
    void ApplyTileStrokeFill(Entity entity, glm::ivec2 hoveredTile, bool erase);
    void EndTileStroke();
    void UndoTileStroke();
    void RedoTileStroke();
    bool IsUndoShortcutPressed() const;
    bool IsRedoShortcutPressed() const;

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


    Scene m_Scene;
    Player m_Player;
    EditorMode m_EditorMode = EditorMode::Editing;
    std::string m_SceneFilePath = "Assets/Scenes/SandboxScene.json";
    std::string m_PlayModeSnapshotPath = "Assets/Scenes/.SandboxScene.playmode.json";
    bool m_TileStrokeActive = false;
    TileEditStroke m_ActiveTileStroke;
    std::vector<TileEditStroke> m_UndoTileStrokes;
    std::vector<TileEditStroke> m_RedoTileStrokes;
};
