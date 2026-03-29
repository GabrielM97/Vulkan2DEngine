#pragma once

#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanSwapchain.h"
#include "VulkanRenderPass.h"
#include "VulkanSync.h"
#include "VulkanVertexBuffer.h"

struct QuadCommand
{
    float offset[2];
    float scale[2];
    float tint[4];
};

struct PushConstantData
{
    float offset[2];
    float scale[2];
    float tint[4];
};

class VulkanRenderer
{
public:
    void Init(GLFWwindow* window, int width, int height);
    void Cleanup();

    void BeginFrame();
    void DrawQuad(float x, float y, float width, float height, float r, float g, float b, float a);
    void EndFrame();
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

    VulkanSwapChain m_swapchain;
    VulkanRenderPass m_renderPass;
    VulkanPipeline m_Pipeline;
    VulkanFramebuffer m_Framebuffer;
    VulkanCommandBuffer m_CommandBuffer;

    VulkanSync m_Sync;
    VulkanVertexBuffer m_VertexBuffer;
    VulkanIndexBuffer m_IndexBuffer;

    uint32_t m_CurrentFrame = 0;

    // Tracks which fence is currently using each swapchain image.
    std::vector<VkFence> m_ImagesInFlight;

    // One render-finished semaphore per swapchain image.
    std::vector<VkSemaphore> m_RenderFinishedPerImage;
    
    std::vector<QuadCommand> m_QuadCommands;
};
