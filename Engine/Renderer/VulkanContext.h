#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

class VulkanContext
{
public:
    void Init(GLFWwindow* window);
    void Cleanup();

    VkInstance GetInstance() const { return instance; }
    VkSurfaceKHR GetSurface() const { return surface; }

private:
    void CreateInstance();
    void CreateSurface(GLFWwindow* window);

    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
};
