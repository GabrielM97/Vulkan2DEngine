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
    
    std::pair<int, int> GetFramebufferSize() const ;
    bool WasResized() const;
    void ResetResizeFlag();

private:
    
    static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);
    
    int width, height;
    std::string title;
    GLFWwindow* window = nullptr;
    bool framebufferResized = false;
};
