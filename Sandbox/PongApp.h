#pragma once

#include <string>

#include "Core/Application.h"
#include "Editor/EditorSceneController.h"
#include "Editor/TileMapEditorController.h"
#include "Object/Player.h"
#include "Pong/Ball.h"
#include "Scene/Scene.h"

class PongApp : public Application
{
protected:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(VulkanRenderer& renderer) override;
    Scene* GetEditorScene() override { return &m_Scene; }
    std::string GetPlayModeSnapshotPath() const override { return "Assets/Scenes/.SandboxScene.playmode.json"; }
    void OnEnterPlayMode() override;
    void OnExitPlayMode() override;

private:
    void CreateDefaultScene();
    void RefreshRuntimeHandles();

    Scene m_Scene;
    Ball m_ball;
    std::string m_SceneFilePath = "Assets/Scenes/SandboxScene.json";
    EditorSceneController m_EditorSceneController;
    TileMapEditorController m_TileMapEditorController;
};
