#pragma once

#include "Core/Application.h"
#include "Scene/Scene.h"

#include <vector>

class SandboxApp : public Application
{
protected:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(VulkanRenderer& renderer) override;

private:
    Scene m_Scene;
};