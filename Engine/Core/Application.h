#pragma once
#include <memory>

#include "Renderer/VulkanContext.h"
#include "Renderer/VulkanDevice.h"
#include "Renderer/VulkanRenderPass.h"
#include "Renderer/VulkanSwapChain.h"

class Window;

class Application {
public:
    Application();
    ~Application();

    bool Init();
    void Run();

private:
    std::unique_ptr<Window> window = nullptr;
    VulkanContext context;
    VulkanDevice device;
    VulkanSwapChain swapchain;
    VulkanRenderPass renderPass;
};
