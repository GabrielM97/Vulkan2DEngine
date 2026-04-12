#pragma once

#include "Scene/GameObject.h"

#include <array>

class Scene;

class SceneEditorPanel
{
public:
    void Draw(Scene& scene);

private:
    void SelectObject(Scene& scene, GameObjectID id);
    void SyncInspectorFromSelection(Scene& scene);
    void DrawHierarchyPanel(Scene& scene);
    void DrawHierarchyNode(Scene& scene, const GameObject& object);
    void DrawInspectorPanel(Scene& scene);

    GameObjectID m_SelectedObjectID = 0;
    GameObjectID m_InspectorObjectID = 0;

    std::array<char, 256> m_NameBuffer{};
    std::array<char, 260> m_TexturePathBuffer{};
    int m_ReparentTargetID = 0;
    int m_SourceRectValues[4] = {0, 0, 0, 0};
    int m_SourceGridValues[4] = {0, 0, 64, 64};
};
