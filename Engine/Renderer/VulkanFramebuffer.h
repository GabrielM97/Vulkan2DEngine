#pragma once
#include <vector>

#include <vulkan/vulkan.h>

class VulkanFramebuffer
{
public:
    void Init(
        VkDevice device,
        VkRenderPass renderPass,
        const std::vector<VkImageView>& imageViews,
        VkExtent2D extent);

    void Cleanup(VkDevice device);

    const std::vector<VkFramebuffer>& Get() const { return m_Framebuffers; }

private:
    std::vector<VkFramebuffer> m_Framebuffers;
};
