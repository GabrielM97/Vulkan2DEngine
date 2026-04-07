#pragma once
#include <array>
#include <vector>
#include <vulkan/vulkan_core.h>

class VulkanDevice;

struct Vertex
{
    float pos[3];
    float colour[3];
    float uv[2];

    static VkVertexInputBindingDescription GetVertexInputBindingDescription()
    {
        VkVertexInputBindingDescription binding{};
        binding.binding = 0;
        binding.stride = sizeof(Vertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return binding;
    }

    static std::array<VkVertexInputAttributeDescription, 3> GetVertexInputAttributeDescription()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributes{};

        // Position
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributes[0].offset = offsetof(Vertex, pos);

        // Color
        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributes[1].offset = offsetof(Vertex, colour);
        
        attributes[2].binding = 0;
        attributes[2].location = 2;
        attributes[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[2].offset = offsetof(Vertex, uv);

        return attributes;
    }
};

class VulkanVertexBuffer
{
public:
    void Init(VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
              const std::vector<Vertex>& vertices);
    void Cleanup(VkDevice device);

    VkBuffer GetBuffer() const { return m_VertexBuffer; }
    uint32_t GetVertexCount() const { return static_cast<uint32_t>(m_VertexCount); }

private:
    void CreateVertexBuffer(VulkanDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
                        const std::vector<Vertex>& vertices);
    
    VkBuffer m_VertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_VertexBufferMemory = VK_NULL_HANDLE;
    size_t m_VertexCount = 0;
};
