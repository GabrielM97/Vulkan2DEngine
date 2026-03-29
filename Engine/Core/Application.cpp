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
        
        vulkanRenderer.BeginFrame();

        // Temporary test quads in clip-space coordinates.
        vulkanRenderer.DrawQuad(0.5f, 0.f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f);
        vulkanRenderer.DrawQuad( 0.1f, -0.9f, 0.3f, 0.3f, 0.0f, 1.0f, 0.0f, 1.0f);
        vulkanRenderer.DrawQuad(-0.4f,  0.2f, 0.5f, 0.25f, 0.0f, 0.0f, 1.0f, 1.0f);
        vulkanRenderer.EndFrame();
    }
}
