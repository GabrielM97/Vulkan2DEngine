#pragma once

#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanSync.h"
#include "VulkanVertexBuffer.h"

class VulkanRenderer
{
public:
    void Init(GLFWwindow* window, int width, int height);
    void Cleanup();
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void DrawFrame();

private:
    VulkanContext context;
    VulkanDevice device;
    VulkanSwapChain swapchain;
    VulkanRenderPass renderPass;

    VulkanPipeline m_Pipeline;
    VulkanFramebuffer m_Framebuffer;
    VulkanCommandBuffer m_CommandBuffer;
    VulkanSync m_Sync;

    uint32_t m_CurrentFrame = 0;

    std::vector<VkFence> m_ImagesInFlight;
    std::vector<VkSemaphore> m_RenderFinishedPerImage;
    VulkanVertexBuffer m_VertexBuffer;
};
