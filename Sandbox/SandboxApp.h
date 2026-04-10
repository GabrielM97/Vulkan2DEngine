#pragma once

#include "Core/Application.h"
#include "Scene/GameObject.h"

#include <vector>

class SandboxApp : public Application
{
protected:
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(VulkanRenderer& renderer) override;

private:
    std::vector<GameObject> m_GameObjects;
};