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

        vulkanRenderer.DrawQuad(glm::vec2(50, 100), glm::vec2(100.0f, 100.0f) , 30.f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        vulkanRenderer.DrawQuad(glm::vec2(400, 100), glm::vec2(200.0f, 150.0f) , 120.f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        vulkanRenderer.DrawQuad(glm::vec2(70, 300), glm::vec2(200.0f, 200.0f) , 75.f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        vulkanRenderer.EndFrame();
    }
}
