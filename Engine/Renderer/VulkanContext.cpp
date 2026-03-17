#include "VulkanContext.h"

#include <iostream>

void VulkanContext::Init(GLFWwindow* window)
{
    CreateInstance();
    CreateSurface(window);
    
    std::cout << "Vulkan Initialized Correctly\n";
}

void VulkanContext::Cleanup()
{
    if(surface)
        vkDestroySurfaceKHR(instance, surface, nullptr);

    if(instance)
        vkDestroyInstance(instance, nullptr);
}

VkInstance VulkanContext::CreateInstance()
{
    //app metadata
    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "Vulkan Engine";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    applicationInfo.pEngineName = "Vulkan";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    applicationInfo.apiVersion = VK_API_VERSION_1_3;
    
    //Vulkan instance extensions are needed to create a Vulkan surface for that window.
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    //vulkan instance metadata
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    
    //debug validation 
#ifndef NDEBUG
    const char* validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
    };

    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;
#else
    createInfo.enabledLayerCount = 0;
#endif
    
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }
    
}

VkSurfaceKHR VulkanContext::CreateSurface(GLFWwindow* window)
{
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface!");
    }
}
