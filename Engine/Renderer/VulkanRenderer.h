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
    void DrawFrame();
    
    void OnFramebufferResized(int width, int height);
    

private:
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    
    // Long-lived setup: survives swapchain recreation.
    void CreatePersistentResources(GLFWwindow* window);
    void DestroyPersistentResources();

    // Swapchain-dependent setup: rebuilt on resize/out-of-date.
    void CreateSwapchainResources(int width, int height);
    void DestroySwapchainResources();
    
    void RecreateSwapchain();
    void RecreateSwapchain(int width, int height);

    void CreatePerImageSyncObjects();
    void DestroyPerImageSyncObjects();
    
    
    GLFWwindow* m_Window = nullptr;

    VulkanContext context;
    VulkanDevice device;

    VulkanSwapChain swapchain;
    VulkanRenderPass renderPass;
    VulkanPipeline m_Pipeline;
    VulkanFramebuffer m_Framebuffer;
    VulkanCommandBuffer m_CommandBuffer;

    VulkanSync m_Sync;
    VulkanVertexBuffer m_VertexBuffer;

    uint32_t m_CurrentFrame = 0;

    // Tracks which fence is currently using each swapchain image.
    std::vector<VkFence> m_ImagesInFlight;

    // One render-finished semaphore per swapchain image.
    std::vector<VkSemaphore> m_RenderFinishedPerImage;
};
