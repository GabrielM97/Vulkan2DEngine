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
    void Shutdown();

protected:
    virtual void OnInit() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender(VulkanRenderer& renderer) {}
    virtual void OnShutdown() {}

    VulkanRenderer& GetRenderer() { return vulkanRenderer; }

    bool IsKeyDown(int key) const;

private:
    std::unique_ptr<Window> window = nullptr;
    VulkanRenderer vulkanRenderer;
    
    bool isRunning = false;
    bool isShutdown = false;
};