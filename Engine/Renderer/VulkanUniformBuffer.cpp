#include "VulkanUniformBuffer.h"

#include <cstring>
#include <stdexcept>

#include "VulkanDevice.h"

void VulkanUniformBuffer::Init(VulkanDevice device, VkDeviceSize size)
{
    m_Size = size;

    device.CreateBuffer(
        size,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_Buffer,
        m_Memory
    );
}

void VulkanUniformBuffer::Cleanup(VkDevice device)
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

    m_Size = 0;
}

void VulkanUniformBuffer::Update(VkDevice device, const void* data, VkDeviceSize size)
{
    if (size > m_Size)
        throw std::runtime_error("Uniform buffer update exceeds allocated size");

    void* mappedData = nullptr;
    vkMapMemory(device, m_Memory, 0, size, 0, &mappedData);
    memcpy(mappedData, data, static_cast<size_t>(size));
    vkUnmapMemory(device, m_Memory);
}