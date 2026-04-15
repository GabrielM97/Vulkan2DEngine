#pragma once

#include <memory>
#include <imgui.h>

#include "Editor/SceneViewportPanel.h"
#include "Editor/TileMapEditorPanel.h"

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
    
    TileMapEditorPanel& GetTileMapEditorPanel() { return m_TileMapEditorPanel; }
    const TileMapEditorPanel& GetTileMapEditorPanel() const { return m_TileMapEditorPanel; }

private:
    void DrawDockspace();
    void DrawTopBar(bool isPlaying);
    float GetTopBarHeight() const { return 40.0f; }

    TileMapEditorPanel m_TileMapEditorPanel;
    std::unique_ptr<SceneViewportPanel> m_SceneViewportPanel;
    std::unique_ptr<SceneEditorPanel> m_SceneEditorPanel;
    bool m_PlayRequested = false;
    bool m_StopRequested = false;
};
