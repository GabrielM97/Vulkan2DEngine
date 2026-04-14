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

private:
    void CreateDefaultScene();
    void RefreshRuntimeHandles();

    Scene m_Scene;
    Player m_Player;
    std::string m_SceneFilePath = "Assets/Scenes/SandboxScene.json";
    bool m_SavePressedLastFrame = false;
    bool m_LoadPressedLastFrame = false;
};
