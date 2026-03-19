#pragma once
#include <string>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool Init();
    bool ShouldClose() const;
    void PollEvents() const;
    GLFWwindow* GetNativeWindow() const;

private:
    int width, height;
    std::string title;
    GLFWwindow* window = nullptr;
};
