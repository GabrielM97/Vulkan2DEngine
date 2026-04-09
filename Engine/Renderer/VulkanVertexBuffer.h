#pragma once
#include <array>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vulkan/vulkan_core.h>

class VulkanDevice;

struct Vertex
{
    glm::vec3 pos{0.0f};
    glm::vec2 uv{0.0f};

    static VkVertexInputBindingDescription GetVertexInputBindingDescription()
    {
        VkVertexInputBindingDescription binding{};
        binding.binding = 0;
        binding.stride = sizeof(Vertex);
        binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return binding;
    }

    static std::array<VkVertexInputAttributeDescription, 2> GetVertexInputAttributeDescription()
    {
        std::array<VkVertexInputAttributeDescription, 2> attributes{};

        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributes[0].offset = offsetof(Vertex, pos);

        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[1].offset = offsetof(Vertex, uv);

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
