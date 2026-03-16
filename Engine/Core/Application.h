#pragma once

class Window;
class VulkanContext;

class Application {
public:
    Application();
    ~Application();

    bool Init();
    void Run();

private:
    Window* window = nullptr;
    VulkanContext* context = nullptr;
};
