#pragma once

#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanSync.h"
#include "VulkanVertexBuffer.h"
#include "VulkanQuadInstanceBuffer.h"
#include "RenderTypes.h"
#include "VulkanUniformBuffer.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "VulkanTexture.h"
#include "Scene/Camera2D.h"

class VulkanRenderer
{
public:
    void Init(GLFWwindow* window, int width, int height);
    void Cleanup();

    uint32_t LoadTexture(const char* path);
    void SetCamera(const Camera2D& camera);

    void BeginFrame();
    void DrawQuad(glm::vec2 position, glm::vec2 size, float rotationDegrees, glm::vec2 uvMin, glm::vec2 uvMax, glm::vec4 tint =
                      glm::vec4(1.0f), uint32_t textureIndex = 0);
    void EndFrame();
    void DrawFrame();
    
    void OnFramebufferResized(int width, int height);
    
    uint32_t GetOrLoadTexture(const std::string& path);
    const VulkanTexture* GetTexture(uint32_t index) const;

private:
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    
    // Long-lived setup: survives swapchain recreation.
    void CreatePersistentResources(GLFWwindow* window);
    void DestroyPersistentResources();
    
    void CreateGlobalResources(int width, int height);
    void DestroyGlobalResources();

    void CreateDescriptorSetLayout();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    void RebuildTextureDescriptorResources();
    void CreateFallbackTexture();
    void UpdateCameraMatrices();

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
    VkCommandPool m_UploadCommandPool = VK_NULL_HANDLE;
    std::vector<VulkanTexture> m_Textures;

    Camera2D m_Camera;
    int m_FramebufferWidth = 1;
    int m_FramebufferHeight = 1;

    VulkanSync m_Sync;
    VulkanVertexBuffer m_VertexBuffer;
    VulkanIndexBuffer m_IndexBuffer;
    VulkanUniformBuffer m_GlobalUniformBuffer;
    VulkanQuadInstanceBuffer m_InstanceBuffer;
    std::vector<QuadInstanceData> m_InstanceData;

    VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> m_TextureDescriptorSets;
    std::vector<PreparedBatch> m_PreparedBatches;

    uint32_t m_CurrentFrame = 0;

    // Tracks which fence is currently using each swapchain image.
    std::vector<VkFence> m_ImagesInFlight;
    // One render-finished semaphore per swapchain image.
    std::vector<VkSemaphore> m_RenderFinishedPerImage;
    std::vector<QuadCommand> m_QuadCommands;
    
    std::unordered_map<std::string, uint32_t> m_TextureCache;
    uint32_t m_FallbackTextureIndex = 0;
    bool m_HasFallbackTexture = false;

};
