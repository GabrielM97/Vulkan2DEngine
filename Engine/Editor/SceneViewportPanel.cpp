#include "Editor/SceneViewportPanel.h"

#include <imgui.h>

void SceneViewportPanel::Draw(ImTextureID textureID)
{
    ImGui::Begin("Scene Viewport");

    const ImVec2 available = ImGui::GetContentRegionAvail();

    m_State.width = available.x > 0.0f ? static_cast<uint32_t>(available.x) : 0;
    m_State.height = available.y > 0.0f ? static_cast<uint32_t>(available.y) : 0;
    m_State.hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
    m_State.focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
    m_State.visible = (m_State.width > 0 && m_State.height > 0);
    
    if (m_State.visible && textureID != NULL)
        ImGui::Image(textureID, ImVec2((float)m_State.width, (float)m_State.height));
    else
    {
        ImGui::Text("Viewport size: %u x %u", m_State.width, m_State.height);
        ImGui::Text("Hovered: %s", m_State.hovered ? "Yes" : "No");
        ImGui::Text("Focused: %s", m_State.focused ? "Yes" : "No");
        ImGui::Text("Visible: %s", m_State.visible ? "Yes" : "No");
        ImGui::Separator();
        ImGui::TextUnformatted("No scene texture.");
    }
    
    ImGui::End();
}
