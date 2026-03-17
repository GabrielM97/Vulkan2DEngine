#include "Application.h"
#include <iostream>

#include "Platform/Window.h"
#include "Renderer/VulkanContext.h"
#include "Renderer/VulkanDevice.h"

Application::Application()
{
    window = std::make_unique<Window>(800, 600, "Vulkan Engine Window");
}

Application::~Application() {
    renderPass.Cleanup(device.GetDevice());
    swapchain.Cleanup(device.GetDevice());
    device.Cleanup();
    context.Cleanup();
}

bool Application::Init() {
    
    if(!window->Init())
        return false;

    context.Init(window->GetNativeWindow());
    device.Init(context);
    
    swapchain.Init(device, context.GetSurface(), 1280, 700);

    renderPass.Init(device.GetDevice(), swapchain.GetFormat());
   
    return true;
}

void Application::Run() {
    while(!window->ShouldClose()) {
        window->PollEvents();
        //context->DrawFrame();
    }
}