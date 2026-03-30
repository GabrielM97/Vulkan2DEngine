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
#include "VulkanUniformBuffer.h"
#include <glm/mat4x4.hpp>

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

struct GlobalUBO
{
    glm::mat4 projection{1.0f};
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
    
    void CreateGlobalResources(int width, int height);
    void DestroyGlobalResources();

    void CreateDescriptorSetLayout();
    void CreateDescriptorPool();
    void CreateDescriptorSet();
    void UpdateProjectionMatrix(int width, int height);

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
    VulkanUniformBuffer m_GlobalUniformBuffer;

    VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
    VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;

    uint32_t m_CurrentFrame = 0;

    // Tracks which fence is currently using each swapchain image.
    std::vector<VkFence> m_ImagesInFlight;
    // One render-finished semaphore per swapchain image.
    std::vector<VkSemaphore> m_RenderFinishedPerImage;
    std::vector<QuadCommand> m_QuadCommands;
};
