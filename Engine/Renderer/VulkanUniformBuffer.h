#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice;

class VulkanUniformBuffer
{
public:
    void Init(VulkanDevice device, VkDeviceSize size);
    void Cleanup(VkDevice device);

    void Update(VkDevice device, const void* data, VkDeviceSize size);

    VkBuffer GetBuffer() const { return m_Buffer; }
    VkDeviceSize GetSize() const { return m_Size; }

private:
    VkBuffer m_Buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_Memory = VK_NULL_HANDLE;
    VkDeviceSize m_Size = 0;
};
