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
#include "VulkanViewportRenderTarget.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <imgui.h>

#include "IRenderer2D.h"
#include "VulkanTexture.h"
#include "Scene/Camera2D.h"

class ImGuiLayer;

class VulkanRenderer : public IRenderer2D
{
public:
    void Init(GLFWwindow* window, int width, int height);
    void Cleanup();

    uint32_t LoadTexture(const char* path);
    void SetCamera(const Camera2D& camera);
    
    void DrawSprite(const Transform2D& transform, const SpriteRenderer& sprite) override;

    void BeginFrame();
    void EndFrame();
    void DrawFrame();
    
    void OnFramebufferResized(int width, int height);
    int GetFramebufferWidth() const { return m_FramebufferWidth; }
    int GetFramebufferHeight() const { return m_FramebufferHeight; }
    
    void SetImGuiRenderCallback(std::function<void(VkCommandBuffer)> callback)
    {
        m_ImGuiRenderCallback = std::move(callback);
    }

    uint32_t GetOrLoadTexture(const std::string& path);
    const VulkanTexture* GetTexture(uint32_t index) const;
    VkInstance GetInstance() const { return context.GetInstance(); }
    VkPhysicalDevice GetPhysicalDevice() const { return device.GetPhysicalDevice(); }
    VkDevice GetDevice() const { return device.GetDevice(); }
    VkQueue GetGraphicsQueue() const { return device.GetGraphicsQueue(); }
    uint32_t GetGraphicsQueueFamily() const { return device.GetGraphicsQueueFamily(); }
    VkRenderPass GetRenderPass() const { return m_renderPass.Get(); }
    VkCommandPool GetUploadCommandPool() const { return m_UploadCommandPool; }
    uint32_t GetMinImageCount() const { return 2; }
    uint32_t GetImageCount() const { return static_cast<uint32_t>(m_swapchain.GetImageViews().size()); }
    
    void EnsureSceneViewportTarget(uint32_t width, uint32_t height);
    uint32_t GetSceneViewportWidth() const { return m_SceneViewportTarget.GetWidth(); }
    uint32_t GetSceneViewportHeight() const { return m_SceneViewportTarget.GetHeight(); }
    
    void SetImGuiLayer(ImGuiLayer* imguiLayer);
    void RefreshSceneViewportTextureHandle();
    ImTextureID GetSceneViewportTextureID() const { return m_SceneViewportTextureID; }

private:
    void DrawQuad(glm::vec2 position, glm::vec2 size, float rotationDegrees, glm::vec2 pivot, glm::vec2 uvMin, glm::vec2 uvMax, 
                    glm::vec4 tint =glm::vec4(1.0f), uint32_t textureIndex = 0);
    
    void RecordSceneViewportPass(VkCommandBuffer commandBuffer);
    void RecordSwapchainUiPass(VkCommandBuffer commandBuffer, uint32_t imageIndex);
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

    std::function<void(VkCommandBuffer)> m_ImGuiRenderCallback;
    VulkanViewportRenderTarget m_SceneViewportTarget;

    Camera2D m_Camera;
    bool m_CameraDirty = true;
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
    
    ImGuiLayer* m_ImGuiLayer = nullptr;
    ImTextureID m_SceneViewportTextureID = 0;

};
