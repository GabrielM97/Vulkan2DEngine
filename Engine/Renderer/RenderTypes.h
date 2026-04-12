#pragma once
#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

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
    glm::vec2 pivot{0.5f, 0.5f};
    glm::vec2 uvMin{0.0f};
    glm::vec2 uvMax{1.0f};
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
    glm::vec2 pivot{0.5f, 0.5f};
    glm::vec2 uvMin{0.0f};
    glm::vec2 uvMax{1.0f};
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

    static std::array<VkVertexInputAttributeDescription, 8> GetAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 8> attributes{};

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
        attributes[3].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[3].offset = offsetof(QuadInstanceData, pivot);

        attributes[4].binding = 1;
        attributes[4].location = 6;
        attributes[4].format = VK_FORMAT_R32_SFLOAT;
        attributes[4].offset = offsetof(QuadInstanceData, textureIndex);

        attributes[5].binding = 1;
        attributes[5].location = 7;
        attributes[5].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[5].offset = offsetof(QuadInstanceData, uvMin);

        attributes[6].binding = 1;
        attributes[6].location = 8;
        attributes[6].format = VK_FORMAT_R32G32_SFLOAT;
        attributes[6].offset = offsetof(QuadInstanceData, uvMax);

        attributes[7].binding = 1;
        attributes[7].location = 9;
        attributes[7].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attributes[7].offset = offsetof(QuadInstanceData, tint);

        return attributes;
    }
};
