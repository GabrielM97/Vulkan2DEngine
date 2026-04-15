#pragma once

#include <memory>
#include <imgui.h>

#include "Editor/SceneViewportPanel.h"

class Scene;
class SceneViewportPanel;
class SceneEditorPanel;

class EditorLayer
{
public:
    EditorLayer();
    ~EditorLayer();

    void Draw(Scene* scene, ImTextureID sceneViewportTextureID, bool isPlaying);
    const SceneViewportState& GetSceneViewportState() const;
    bool ConsumePlayRequest();
    bool ConsumeStopRequest();

private:
    void DrawDockspace();
    void DrawTopBar(bool isPlaying);
    float GetTopBarHeight() const { return 40.0f; }

    std::unique_ptr<SceneViewportPanel> m_SceneViewportPanel;
    std::unique_ptr<SceneEditorPanel> m_SceneEditorPanel;
    bool m_PlayRequested = false;
    bool m_StopRequested = false;
};
