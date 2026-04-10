#pragma once
#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

struct Camera2D
{
    glm::vec2 position{ 0.0f, 0.0f };
    float zoom = 1.0f;
};

struct PreparedBatch
{
    uint32_t textureIndex = 0;
    uint32_t firstInstance = 0;
    uint32_t instanceCount = 0;
};

struct QuadCommand
{
    glm::vec2 position{0.0f};
    glm::vec2 size{1.0f};
    float rotation = 0.0f;
    glm::vec4 tint{1.0f};
    uint32_t textureIndex = 0;
};

struct GlobalUBO
{
    glm::mat4 viewProjection{1.0f};
};

struct QuadInstanceData
{
    glm::vec2 position{0.0f};
    glm::vec2 size{1.0f};
    float rotation = 0.0f;
    float textureIndex = 0.0f;
    glm::vec4 tint{1.0f};

    static VkVertexInputBindingDescription GetBindingDescription()
    {
        VkVertexInputBindingDescription binding{};
        binding.binding = 1;
        binding.stride = sizeof(QuadInstanceData);
        binding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
        return binding;
    }

    static std::array<VkVertexInputAttributeDescription, 5> GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 5> attributes{};

        attributes[0].binding = 1;
        attributes[0].location = 2;
        attributes[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[0].offset = offsetof(QuadInstanceData, position);

        attributes[1].binding = 1;
        attributes[1].location = 3;
        attributes[1].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[1].offset = offsetof(QuadInstanceData, size);

        attributes[2].binding = 1;
        attributes[2].location = 4;
        attributes[2].format = VK_FORMAT_R32_SFLOAT;
        attributes[2].offset = offsetof(QuadInstanceData, rotation);

        attributes[3].binding = 1;
        attributes[3].location = 5;
        attributes[3].format = VK_FORMAT_R32_SFLOAT;
        attributes[3].offset = offsetof(QuadInstanceData, textureIndex);

        attributes[4].binding = 1;
        attributes[4].location = 6;
        attributes[4].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributes[4].offset = offsetof(QuadInstanceData, tint);

        return attributes;
    }
};
