#pragma once
#include <vulkan/vulkan.h>

class VulkanRenderPass
{
public:
    void Init(VkDevice device, VkFormat swapchainFormat);
    void Cleanup(VkDevice device);

    VkRenderPass Get() const { return renderPass; }

private:
    VkRenderPass renderPass = VK_NULL_HANDLE;
};
