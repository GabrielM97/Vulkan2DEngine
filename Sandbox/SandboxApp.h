#pragma once

#include <string>

#include "Core/Application.h"
#include "Object/Player.h"
#include "Scene/Scene.h"

class SandboxApp : public Application
{
protected:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(VulkanRenderer& renderer) override;
    Scene* GetEditorScene() override { return &m_Scene; }
    bool IsEditorPlaying() const override;
    void OnEditorPlay() override;
    void OnEditorStop() override;

private:
    enum class EditorMode
    {
        Editing,
        Playing
    };

    void CreateDefaultScene();
    void RefreshRuntimeHandles();
    void EnterPlayMode();
    void ExitPlayMode();
    bool TryGetHoveredTile(Entity entity, glm::ivec2& outTile) const;
    glm::vec2 ScreenToWorld(const glm::vec2& screenPosition, const SceneViewportState& viewportState) const;


    Scene m_Scene;
    Player m_Player;
    EditorMode m_EditorMode = EditorMode::Editing;
    std::string m_SceneFilePath = "Assets/Scenes/SandboxScene.json";
    std::string m_PlayModeSnapshotPath = "Assets/Scenes/.SandboxScene.playmode.json";
};
