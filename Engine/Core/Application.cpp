#include "Application.h"

#include <GLFW/glfw3.h>
#include <chrono>

Application::Application()
{
    window = std::make_unique<Window>(800, 600, "Vulkan Engine Window");
}

Application::~Application()
{
    OnShutdown();
    vulkanRenderer.Cleanup();
}

bool Application::Init()
{
    if (!window->Init())
        return false;

    auto [framebufferWidth, framebufferHeight] = window->GetFramebufferSize();

    vulkanRenderer.Init(
        window->GetNativeWindow(),
        framebufferWidth,
        framebufferHeight
    );

    OnInit();

    return true;
}

bool Application::IsKeyDown(int key) const
{
    return glfwGetKey(window->GetNativeWindow(), key) == GLFW_PRESS;
}

void Application::Run()
{
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!window->ShouldClose())
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        float deltaTime = std::chrono::duration<float>(
            currentTime - lastTime
        ).count();

        lastTime = currentTime;

        window->PollEvents();

        if (window->WasResized())
        {
            auto [framebufferWidth, framebufferHeight] = window->GetFramebufferSize();

            vulkanRenderer.OnFramebufferResized(
                framebufferWidth,
                framebufferHeight
            );

            window->ResetResizeFlag();
        }

        OnUpdate(deltaTime);

        vulkanRenderer.BeginFrame();

        OnRender(vulkanRenderer);

        vulkanRenderer.EndFrame();
    }
}