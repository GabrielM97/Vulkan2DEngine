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

    void Draw(Scene* scene, ImTextureID sceneViewportTextureID);
    const SceneViewportState& GetSceneViewportState() const;

private:
    void DrawDockspace();

    std::unique_ptr<SceneViewportPanel> m_SceneViewportPanel;
    std::unique_ptr<SceneEditorPanel> m_SceneEditorPanel;
};
