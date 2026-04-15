#include "Editor/EditorLayer.h"

#include <imgui.h>

#include "Editor/SceneViewportPanel.h"
#include "Editor/SceneEditorPanel.h"
#include "Renderer/VulkanRenderer.h"
#include "Scene/Scene.h"

EditorLayer::EditorLayer()
    : m_SceneViewportPanel(std::make_unique<SceneViewportPanel>())
    , m_SceneEditorPanel(std::make_unique<SceneEditorPanel>())
{
}

EditorLayer::~EditorLayer() = default;

void EditorLayer::Draw(Scene* scene, VulkanRenderer& renderer, ImTextureID sceneViewportTextureID, bool isPlaying)
{
    DrawDockspace();
    DrawTopBar(isPlaying);

    if (scene == nullptr)
        return;

    m_SceneViewportPanel->Draw(sceneViewportTextureID);
    m_SceneEditorPanel->Draw(*scene);
    m_TileMapEditorPanel.Draw(*scene, renderer, m_SceneEditorPanel->GetSelectedObjectID());
}

const SceneViewportState& EditorLayer::GetSceneViewportState() const
{
    return m_SceneViewportPanel->GetState();
}

bool EditorLayer::ConsumePlayRequest()
{
    const bool requested = m_PlayRequested;
    m_PlayRequested = false;
    return requested;
}

bool EditorLayer::ConsumeStopRequest()
{
    const bool requested = m_StopRequested;
    m_StopRequested = false;
    return requested;
}

void EditorLayer::DrawDockspace()
{
#ifdef IMGUI_HAS_DOCK
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
    const float topBarHeight = GetTopBarHeight();

    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + topBarHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - topBarHeight));
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

void EditorLayer::DrawTopBar(bool isPlaying)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const float topBarHeight = GetTopBarHeight();
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoNavFocus;

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, topBarHeight));
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 8.0f));

    if (ImGui::Begin("EditorTopBar", nullptr, windowFlags))
    {
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(isPlaying ? "Mode: Playing" : "Mode: Editing");
        ImGui::SameLine();

        if (!isPlaying)
        {
            if (ImGui::Button("Play"))
                m_PlayRequested = true;
        }
        else
        {
            if (ImGui::Button("Stop"))
                m_StopRequested = true;
        }
    }

    ImGui::End();
    ImGui::PopStyleVar(3);
}
