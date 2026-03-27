#include "VulkanVertexBuffer.h"
#include <cstring>
#include <stdexcept>

#include "VulkanDevice.h"

void VulkanVertexBuffer::Init(VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
                              const std::vector<Vertex>& vertices)
{
    m_VertexCount = vertices.size();
    CreateVertexBuffer(device, commandPool, graphicsQueue, vertices);
}

void VulkanVertexBuffer::Cleanup(VkDevice device)
{
    if (m_VertexBuffer != VK_NULL_HANDLE)
        vkDestroyBuffer(device, m_VertexBuffer, nullptr);
    if (m_VertexBufferMemory != VK_NULL_HANDLE)
        vkFreeMemory(device, m_VertexBufferMemory, nullptr);
}

void VulkanVertexBuffer::CreateVertexBuffer(VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
                                            const std::vector<Vertex>& vertices)
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    //Create staging buffer in CPU visible memory
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    // createBuffer(size, usage, properties, &buffer, &memory);
    device.CreateBuffer(bufferSize,
                        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        stagingBuffer, stagingBufferMemory);

    // Copy vertex data
    void* data;
    vkMapMemory(device.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.GetDevice(), stagingBufferMemory);

    //Create vertex buffer on GPU local memory
    device.CreateBuffer(bufferSize,
                        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        m_VertexBuffer, m_VertexBufferMemory);

    //Copy from staging buffer to vertex buffer
    device.CopyBuffer(commandPool, graphicsQueue, stagingBuffer, m_VertexBuffer, bufferSize);

    // Cleanup staging buffer
    vkDestroyBuffer(device.GetDevice(), stagingBuffer, nullptr);
    vkFreeMemory(device.GetDevice(), stagingBufferMemory, nullptr);
}
