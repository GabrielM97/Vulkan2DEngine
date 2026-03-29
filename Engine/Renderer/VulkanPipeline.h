#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanPipeline
{
public:
    void Init(VkDevice device, VkExtent2D extent, VkRenderPass renderPass);
    void Cleanup(VkDevice device);

    VkPipeline Get() const { return m_pipeline; }
    VkPipelineLayout GetLayout() const { return m_pipelineLayout; }

private:
    VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);
    std::vector<char> ReadFile(const std::string& filename);

    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
};
