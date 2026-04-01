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

        // Temporary test quads in clip-space coordinates.
        vulkanRenderer.BeginFrame();

        vulkanRenderer.DrawQuad(50.f, 0.0f, 100.0f, 100.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        vulkanRenderer.DrawQuad(240.0f, 80.0f, 180.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f);
        vulkanRenderer.DrawQuad(120.0f, 240.0f, 220.0f, 140.0f, 0.0f, 0.5f, 1.0f, 1.0f);

        vulkanRenderer.EndFrame();
    }
}
