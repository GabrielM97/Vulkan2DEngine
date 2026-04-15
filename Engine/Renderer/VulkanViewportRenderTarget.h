#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice;

class VulkanViewportRenderTarget
{
public:
    void Init(VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format);
    void Cleanup(VkDevice device);
    void Resize(VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format);

    void TransitionToShaderRead(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
    void TransitionToColorAttachment(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
    
    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }
    bool IsValid() const { return m_Image != VK_NULL_HANDLE; }

    VkImage GetImage() const { return m_Image; }
    VkImageView GetImageView() const { return m_ImageView; }
    VkSampler GetSampler() const { return m_Sampler; }
    VkFormat GetFormat() const { return m_Format; }
    VkRenderPass GetRenderPass() const { return m_RenderPass; }
    VkFramebuffer GetFramebuffer() const { return m_Framebuffer; }
    VkExtent2D GetExtent() const { return VkExtent2D{ m_Width, m_Height }; }

private:
    void CreateImage(VulkanDevice& device);
    void CreateImageView(VkDevice device);
    void CreateSampler(VkDevice device);
    
    void CreateRenderPass(VkDevice device);
    void CreateFramebuffer(VkDevice device);
    void TransitionLayout(
        VkDevice device,
        VkCommandPool commandPool,
        VkQueue graphicsQueue,
        VkImageLayout oldLayout,
        VkImageLayout newLayout,
        VkAccessFlags srcAccessMask,
        VkAccessFlags dstAccessMask,
        VkPipelineStageFlags srcStage,
        VkPipelineStageFlags dstStage);

    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    VkFormat m_Format = VK_FORMAT_UNDEFINED;

    VkImage m_Image = VK_NULL_HANDLE;
    VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
    VkImageView m_ImageView = VK_NULL_HANDLE;
    VkSampler m_Sampler = VK_NULL_HANDLE;
    
    VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;
};
