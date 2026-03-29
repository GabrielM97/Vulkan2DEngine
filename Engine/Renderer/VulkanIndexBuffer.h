#pragma once

#include <vector>
#include <vulkan/vulkan.h>

class VulkanDevice;

class VulkanIndexBuffer
{
public:
    void Init(
        VulkanDevice device,
        VkCommandPool commandPool,
        VkQueue graphicsQueue,
        const std::vector<uint32_t>& indices
    );

    void Cleanup(VkDevice device);

    VkBuffer GetBuffer() const { return m_IndexBuffer; }
    uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_IndexCount); }

private:
    void CreateIndexBuffer(
        VulkanDevice device,
        VkCommandPool commandPool,
        VkQueue graphicsQueue,
        const std::vector<uint32_t>& indices
    );

private:
    VkBuffer m_IndexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_IndexBufferMemory = VK_NULL_HANDLE;
    size_t m_IndexCount = 0;
};