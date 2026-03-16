#include "Window.h"
#include <iostream>

Window::Window(int w, int h, const std::string& t) : width(w), height(h), title(t) {}

Window::~Window() {
    if(window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

bool Window::Init() {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // We use Vulkan, not OpenGL

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create GLFW window\n";
        return false;
    }

    return true;
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::PollEvents() const {
    glfwPollEvents();
}

GLFWwindow* Window::GetNativeWindow() const {
    return window;
}
