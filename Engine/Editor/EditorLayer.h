#pragma once

#include <memory>

class Scene;
class SceneEditorPanel;

class EditorLayer
{
public:
    EditorLayer();
    ~EditorLayer();

    void Draw(Scene* scene);

private:
    void DrawDockspace();

    std::unique_ptr<SceneEditorPanel> m_SceneEditorPanel;
};
