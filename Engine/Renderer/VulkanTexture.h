#pragma once
#include <vulkan/vulkan_core.h>
#include "stb_image.h"

/*
    Pipeline barriers are primarily used for synchronizing access to resources:
    - Make sure that an image was written to before it is read
    - Transition image layouts (VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, etc)
    - Transfer queue family ownership when using VK_SHARING_MODE_EXCLUSIVE
*/

class VulkanDevice;

class VulkanTexture
{
public:
    VulkanTexture() = default;
    ~VulkanTexture() = default;

    VulkanTexture(const VulkanTexture&) = delete;
    VulkanTexture& operator=(const VulkanTexture&) = delete;

    VulkanTexture(VulkanTexture&& other) noexcept;
    VulkanTexture& operator=(VulkanTexture&& other) noexcept;

    void Init(VulkanDevice& device, VkCommandPool commandPool, VkQueue graphicsQueue, const char* path);
    void Cleanup(VkDevice device);

    VkImageView GetImageView() const { return m_ImageView; }
    VkSampler GetSampler() const { return m_Sampler; }

private:
    void CreateTextureImage(VulkanDevice& device, VkCommandPool commandPool, VkQueue graphicsQueue, const char* path);
    void CreateImage(VulkanDevice& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    void TransitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void CreateImageView(VkDevice device);
    void CreateSampler(VulkanDevice& device);

    VkImage m_Image = VK_NULL_HANDLE;
    VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
    VkImageView m_ImageView = VK_NULL_HANDLE;
    VkSampler m_Sampler = VK_NULL_HANDLE;
};
