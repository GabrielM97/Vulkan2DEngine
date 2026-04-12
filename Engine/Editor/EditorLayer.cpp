#include "Editor/EditorLayer.h"

#include "Editor/SceneEditorPanel.h"
#include "Scene/Scene.h"

EditorLayer::EditorLayer()
    : m_SceneEditorPanel(std::make_unique<SceneEditorPanel>())
{
}

EditorLayer::~EditorLayer() = default;

void EditorLayer::Draw(Scene* scene)
{
    if (scene == nullptr)
        return;

    m_SceneEditorPanel->Draw(*scene);
}
