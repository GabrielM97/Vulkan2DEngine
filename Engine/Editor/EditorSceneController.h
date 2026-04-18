#pragma once

#include <glm/vec2.hpp>

#include <string>

#include "Core/InputState.h"
#include "Editor/SceneViewportPanel.h"
#include "Math/Transform2D.h"
#include "Scene/SceneComponents.h"

class EditorLayer;
class Entity;
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
    bool TryGetSelectionShape(Entity entity, Transform2D& outTransform, glm::vec2& outSize) const;
    bool TryPickGameObject(
        Scene& scene,
        const InputState& input,
        const SceneViewportState& viewportState,
        GameObjectID& outObjectID) const;

    std::string m_SceneFilePath = "Assets/Scenes/Scene.json";
    bool m_ShowCollisionDebug = false;
};
