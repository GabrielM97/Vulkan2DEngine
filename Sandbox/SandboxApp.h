#pragma once

#include "Core/Application.h"
#include "Scene/Scene.h"

class SandboxApp : public Application
{
protected:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(VulkanRenderer& renderer) override;
    Scene* GetEditorScene() override { return &m_Scene; }

private:
    Scene m_Scene;
    GameObjectHandle m_Player;
};
