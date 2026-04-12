#pragma once

#include <array>

#include "Scene/SceneComponents.h"

class Scene;

class SceneEditorPanel
{
public:
    void Draw(Scene& scene);

private:
    void SelectObject(Scene& scene, GameObjectID id);
    void SyncInspectorFromSelection(Scene& scene);
    void DrawHierarchyPanel(Scene& scene);
    void DrawHierarchyNode(Scene& scene, GameObjectID id);
    void DrawInspectorPanel(Scene& scene);

    GameObjectID m_SelectedObjectID = 0;
    GameObjectID m_InspectorObjectID = 0;

    std::array<char, 256> m_NameBuffer{};
    std::array<char, 260> m_TexturePathBuffer{};
    std::array<char, 260> m_AnimationSetPathBuffer{};
    std::array<char, 128> m_AnimationClipBuffer{};
    int m_ReparentTargetID = 0;
    int m_SourceRectValues[4] = {0, 0, 0, 0};
    int m_SourceGridValues[4] = {0, 0, 64, 64};
};
