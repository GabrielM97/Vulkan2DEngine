#include "VulkanIndexBuffer.h"
#include "VulkanDevice.h"
#include <cstring>

void VulkanIndexBuffer::Init(VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
                             const std::vector<uint32_t>& indices)
{
    m_IndexCount = indices.size();
    CreateIndexBuffer(device, commandPool, graphicsQueue, indices);
}

void VulkanIndexBuffer::Cleanup(VkDevice device)
{
    if (m_IndexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(device, m_IndexBuffer, nullptr);
        m_IndexBuffer = VK_NULL_HANDLE;
    }
    
    if (m_IndexBufferMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(device, m_IndexBufferMemory, nullptr);
        m_IndexBufferMemory = VK_NULL_HANDLE;
    }
}

void VulkanIndexBuffer::CreateIndexBuffer(VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
    const std::vector<uint32_t>& indices)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    
    device.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    
    void* data;
    vkMapMemory(device.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.GetDevice(), stagingBufferMemory);
    
    // Device-local buffer actually used during rendering.
    device.CreateBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        m_IndexBuffer,
        m_IndexBufferMemory
    );

    device.CopyBuffer(commandPool, graphicsQueue, stagingBuffer, m_IndexBuffer, bufferSize);

    vkDestroyBuffer(device.GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(device.GetDevice(), stagingBufferMemory, nullptr);
}
