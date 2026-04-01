#pragma once
#include <memory>

#include "Platform/Window.h"
#include "Renderer/VulkanRenderer.h"

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
