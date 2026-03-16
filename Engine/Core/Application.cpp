#include "Application.h"
#include <iostream>

#include "Platform/Window.h"
#include "Renderer/VulkanContext.h"

Application::Application() {}

Application::~Application() {
    delete context;
    delete window;
}

bool Application::Init() {
    window = new Window(800, 600, "Vulkan Engine Window");
    if(!window->Init())
        return false;

    // context = new VulkanContext(window);
    // if (!context->Init())
    //     return false;

    return true;
}

void Application::Run() {
    while(!window->ShouldClose()) {
        window->PollEvents();
        //context->DrawFrame();
    }
}