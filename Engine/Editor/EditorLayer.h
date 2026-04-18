#pragma once

#include <memory>
#include <array>
#include <imgui.h>
#include <glm/vec2.hpp>

#include "Editor/SceneViewportPanel.h"
#include "Editor/TileSetEditorPanel.h"
#include "Editor/TileMapEditorPanel.h"

class Entity;
class Scene;
class SceneViewportPanel;
class SceneEditorPanel;
class VulkanRenderer;

class EditorLayer
{
public:
    EditorLayer();
    ~EditorLayer();

    void Draw(Scene* scene, VulkanRenderer& renderer, ImTextureID sceneViewportTextureID, bool isPlaying);
    const SceneViewportState& GetSceneViewportState() const;
    bool ConsumePlayRequest();
    bool ConsumeStopRequest();
    GameObjectID GetSelectedObjectID() const { return m_SelectedObjectID; }
    void SetSelectedObjectID(Scene& scene, GameObjectID id);
    
    TileMapEditorPanel& GetTileMapEditorPanel() { return m_TileMapEditorPanel; }
    const TileMapEditorPanel& GetTileMapEditorPanel() const { return m_TileMapEditorPanel; }

private:
    void DrawDockspace();
    void DrawTopBar(bool isPlaying);
    void DrawSelectedObjectOverlay(Scene& scene, bool isPlaying);
    void DrawTileStampPreview(Scene& scene, bool isPlaying);
    bool TryGetSelectionCorners(Entity entity, std::array<glm::vec2, 4>& outCorners) const;
    float GetTopBarHeight() const { return 40.0f; }

    TileSetEditorPanel m_TileSetEditorPanel;
    TileMapEditorPanel m_TileMapEditorPanel;
    std::unique_ptr<SceneViewportPanel> m_SceneViewportPanel;
    std::unique_ptr<SceneEditorPanel> m_SceneEditorPanel;
    GameObjectID m_SelectedObjectID = 0;
    bool m_PlayRequested = false;
    bool m_StopRequested = false;
};
