#pragma once

#include <string>

#include "Core/InputState.h"
#include "Editor/SceneViewportPanel.h"

class EditorLayer;
class Scene;
class VulkanRenderer;

class EditorSceneController
{
public:
    void SetSceneFilePath(const std::string& path) { m_SceneFilePath = path; }

    bool Update(
        Scene& scene,
        EditorLayer& editorLayer,
        const InputState& input,
        const SceneViewportState& viewportState,
        bool isPlaying,
        float deltaTime);

    void Render(Scene& scene, EditorLayer& editorLayer, VulkanRenderer& renderer) const;

private:
    std::string m_SceneFilePath = "Assets/Scenes/Scene.json";
    bool m_ShowCollisionDebug = false;
};
