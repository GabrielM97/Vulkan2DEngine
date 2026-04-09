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

        for (int i = 0; i < 100; ++i)
        {
            for (int j = 0; j < 50; ++j)
            {
                vulkanRenderer.DrawQuad(glm::vec2(60.f*i, 60.f*j), glm::vec2(40.0f, 40.0f) , i*10, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            }
         
        }
        
        vulkanRenderer.EndFrame();
    }
}
