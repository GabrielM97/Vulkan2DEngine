#include "Editor/EditorLayer.h"

#include <imgui.h>

#include "Editor/SceneViewportPanel.h"
#include "Editor/SceneEditorPanel.h"
#include "Scene/Scene.h"

EditorLayer::EditorLayer()
    : m_SceneViewportPanel(std::make_unique<SceneViewportPanel>())
    , m_SceneEditorPanel(std::make_unique<SceneEditorPanel>())
{
}

EditorLayer::~EditorLayer() = default;

void EditorLayer::Draw(Scene* scene, ImTextureID sceneViewportTextureID)
{
    DrawDockspace();

    if (scene == nullptr)
        return;

    m_SceneViewportPanel->Draw(sceneViewportTextureID);
    m_SceneEditorPanel->Draw(*scene);
}

const SceneViewportState& EditorLayer::GetSceneViewportState() const
{
    return m_SceneViewportPanel->GetState();
}

void EditorLayer::DrawDockspace()
{
#ifdef IMGUI_HAS_DOCK
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoNavFocus
        | ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("EditorDockspace", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGui::DockSpace(ImGui::GetID("EditorDockspaceRoot"), ImVec2(0.0f, 0.0f), dockspaceFlags);
    ImGui::End();
#endif
}
