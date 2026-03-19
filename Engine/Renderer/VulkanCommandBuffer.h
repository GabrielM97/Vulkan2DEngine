#pragma once
#include <vector>
#include <vulkan/vulkan.h>

class VulkanCommandBuffer
{
public:
    void Init(VkDevice device, uint32_t queueFamilyIndex, uint32_t count);
    void Cleanup(VkDevice device);

    void Record(
        VkRenderPass renderPass,
        const std::vector<VkFramebuffer>& framebuffers,
        VkExtent2D extent,
        VkPipeline pipeline);

    const std::vector<VkCommandBuffer>& Get() const { return m_CommandBuffers; }

private:
    VkDevice m_Device = VK_NULL_HANDLE;
    VkCommandPool m_CommandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_CommandBuffers;
};
