#pragma once

#include "RenderTypes.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <array>

class VulkanDevice;

class VulkanQuadInstanceBuffer
{
public:
    void Init(VulkanDevice device, size_t maxInstances);
    void Cleanup(VkDevice device);

    void Update(VulkanDevice device, const std::vector<QuadInstanceData>& instances);

    VkBuffer GetBuffer() const { return m_Buffer; }
    uint32_t GetInstanceCount() const { return m_InstanceCount; }

private:
    void CreateBuffer(VulkanDevice device, size_t instanceCapacity);

private:
    VkBuffer m_Buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_Memory = VK_NULL_HANDLE;
    size_t m_Capacity = 0;
    uint32_t m_InstanceCount = 0;
};