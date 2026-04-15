#pragma once

#include <imgui.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

class VulkanRenderer;

class ImGuiLayer
{
public:
    void Init(GLFWwindow* window, VulkanRenderer& renderer);
    void Shutdown();

    void BeginFrame();
    void EndFrame();
    void Render(VkCommandBuffer commandBuffer);
    bool WantsKeyboardCapture() const;
    bool WantsTextInput() const;
    
    ImTextureID RegisterTexture(VkSampler sampler, VkImageView imageView, VkImageLayout layout);
    void UnregisterTexture(ImTextureID textureID);

private:
    void UploadFontTexture(VulkanRenderer& renderer);

    VkDevice m_Device = VK_NULL_HANDLE;
    VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
    bool m_Initialized = false;
};
