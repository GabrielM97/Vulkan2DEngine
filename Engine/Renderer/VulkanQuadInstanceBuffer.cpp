#include "VulkanQuadInstanceBuffer.h"

#include <cstring>
#include <stdexcept>

#include "VulkanDevice.h"

void VulkanQuadInstanceBuffer::Init(VulkanDevice device, size_t maxInstances)
{
    CreateBuffer(device, maxInstances);
}

void VulkanQuadInstanceBuffer::Cleanup(VkDevice device)
{
    if (m_Buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, m_Buffer, nullptr);
        m_Buffer = VK_NULL_HANDLE;
    }

    if (m_Memory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, m_Memory, nullptr);
        m_Memory = VK_NULL_HANDLE;
    }

    m_Capacity = 0;
    m_InstanceCount = 0;
}

void VulkanQuadInstanceBuffer::CreateBuffer(VulkanDevice device, size_t instanceCapacity)
{
    m_Capacity = instanceCapacity;

    VkDeviceSize bufferSize = sizeof(QuadInstanceData) * instanceCapacity;

    device.CreateBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_Buffer,
        m_Memory
    );
}

void VulkanQuadInstanceBuffer::Update(VulkanDevice device, const std::vector<QuadInstanceData>& instances)
{
    m_InstanceCount = static_cast<uint32_t>(instances.size());

    if (instances.empty())
        return;

    if (instances.size() > m_Capacity)
    {
        Cleanup(device.GetDevice());
        CreateBuffer(device, std::max(instances.size(), m_Capacity > 0 ? m_Capacity * 2 : size_t(1)));
    }

    VkDeviceSize bufferSize = sizeof(QuadInstanceData) * instances.size();

    void* data = nullptr;
    vkMapMemory(device.GetDevice(), m_Memory, 0, bufferSize, 0, &data);
    memcpy(data, instances.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(device.GetDevice(), m_Memory);
}
