#include "VulkanCommandBuffer.h"
#include <stdexcept>

void VulkanCommandBuffer::Init(VkDevice device, uint32_t queueFamilyIndex, uint32_t count)
{
    m_Device = device;

    // Create command pool
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create command pool");

    // Allocate command buffers
    m_CommandBuffers.resize(count);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_CommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = count;

    if (vkAllocateCommandBuffers(device, &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate command buffers");
}

void VulkanCommandBuffer::Record(
    VkRenderPass renderPass,
    const std::vector<VkFramebuffer>& framebuffers,
    VkExtent2D extent,
    VkPipeline pipeline)
{
    for (size_t i = 0; i < m_CommandBuffers.size(); i++)
    {
        VkCommandBuffer cmd = m_CommandBuffers[i];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        vkBeginCommandBuffer(cmd, &beginInfo);

        VkClearValue clearColor = {{0.1f, 0.1f, 0.1f, 1.0f}};

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = framebuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = extent;
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

        vkCmdDraw(cmd, 3, 1, 0, 0);

        vkCmdEndRenderPass(cmd);

        vkEndCommandBuffer(cmd);
    }
}

void VulkanCommandBuffer::Cleanup(VkDevice device)
{
    m_CommandBuffers.clear();

    if (m_CommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(device, m_CommandPool, nullptr);
        m_CommandPool = VK_NULL_HANDLE;
    }
}
