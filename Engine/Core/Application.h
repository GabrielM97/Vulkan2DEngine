#pragma once

#include <memory>

#include "Platform/Window.h"
#include "Renderer/VulkanRenderer.h"

class Application
{
public:
    Application();
    virtual ~Application();

    bool Init();
    void Run();

protected:
    virtual void OnInit() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender(VulkanRenderer& renderer) {}
    virtual void OnShutdown() {}

    VulkanRenderer& GetRenderer() { return vulkanRenderer; }

private:
    std::unique_ptr<Window> window = nullptr;
    VulkanRenderer vulkanRenderer;
};