#pragma once
#include <memory>
#include "Renderer/VulkanRenderer.h"

class Window;

class Application
{
public:
    Application();
    ~Application();

    bool Init();
    void Run();

private:
    std::unique_ptr<Window> window = nullptr;
    VulkanRenderer vulkanRenderer;
};
