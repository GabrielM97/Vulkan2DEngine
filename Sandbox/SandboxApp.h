#pragma once

#include "Core/Application.h"
#include "Scene/Scene.h"

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
    Scene m_Scene;
    GameObjectHandle m_Player;
};
