#pragma once
#include <vulkan/vulkan.h>

class VulkanContext;


class VulkanDevice
{
public:
    void Init(VulkanContext& context);

    void Cleanup();

    VkDevice GetDevice() const { return device; };

    VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; };

    VkQueue GetGraphicsQueue() const { return graphicsQueue; }

    VkQueue GetPresentQueue() const { return presentQueue; }


    uint32_t GetGraphicsQueueFamily() const { return graphicsQueueFamilyIndex; }

    uint32_t GetPresentQueueFamily() const { return presentQueueFamilyIndex; }

    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer,
                      VkDeviceMemory& bufferMemory);
    void CopyBuffer(VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer,
                    VkDeviceSize size);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

private:
    void PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

    void CreateLogicalDevice();

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device = VK_NULL_HANDLE;


    VkQueue graphicsQueue = VK_NULL_HANDLE;

    VkQueue presentQueue = VK_NULL_HANDLE;


    uint32_t graphicsQueueFamilyIndex = 0;

    uint32_t presentQueueFamilyIndex = 0;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
};
