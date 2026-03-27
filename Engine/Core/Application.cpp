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

    vulkanRenderer.Init(window->GetNativeWindow(), 1250, 700);

    return true;
}

void Application::Run()
{
    while (!window->ShouldClose())
    {
        window->PollEvents();
        vulkanRenderer.DrawFrame();
    }
}
