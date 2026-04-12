#pragma once

#include "Core/Application.h"
#include "Scene/Scene.h"

#include <array>
#include <vector>
#include <imgui.h>

class SandboxApp : public Application
{
protected:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(VulkanRenderer& renderer) override;

    void OnImGuiUpdate() override;

private:
    void SelectObject(GameObjectID id);
    void SyncInspectorFromSelection();
    void DrawHierarchyPanel();
    void DrawHierarchyNode(const GameObject& object);
    void DrawInspectorPanel();

    Scene m_Scene;
    GameObjectHandle m_Player;
    GameObjectID m_SelectedObjectID = 0;
    GameObjectID m_InspectorObjectID = 0;

    std::array<char, 256> m_NameBuffer{};
    std::array<char, 260> m_TexturePathBuffer{};
    int m_ReparentTargetID = 0;
    int m_SourceRectValues[4] = {0, 0, 0, 0};
    int m_SourceGridValues[4] = {0, 0, 64, 64};
};
