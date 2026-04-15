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

    Scene m_Scene;
    Player m_Player;
    EditorMode m_EditorMode = EditorMode::Editing;
    std::string m_SceneFilePath = "Assets/Scenes/SandboxScene.json";
    std::string m_PlayModeSnapshotPath = "Assets/Scenes/.SandboxScene.playmode.json";
};
