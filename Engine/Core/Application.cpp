#include "Application.h"
#include <iostream>

#include "Platform/Window.h"

Application::Application()
{
    window = std::make_unique<Window>(800, 600, "Vulkan Engine Window");
}

Application::~Application()
{
    vulkanRenderer.Cleanup();
}

bool Application::Init()
{
    if (!window->Init())
        return false;

    auto [framebufferWidth, framebufferHeight] = window->GetFramebufferSize();
    vulkanRenderer.Init(window->GetNativeWindow(), framebufferWidth, framebufferHeight);

    return true;
}

void Application::Run()
{
    while (!window->ShouldClose())
    {
        window->PollEvents();
        if (window->WasResized())
        {
            auto [framebufferWidth, framebufferHeight] = window->GetFramebufferSize();
            vulkanRenderer.OnFramebufferResized(framebufferWidth, framebufferHeight);
            window->ResetResizeFlag();
            continue;
        }
        vulkanRenderer.DrawFrame();
    }
}
