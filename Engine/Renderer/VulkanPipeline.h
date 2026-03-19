#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

class VulkanPipeline
{
public:
    void Init(VkDevice device, VkExtent2D extent, VkRenderPass renderPass);
    void Cleanup(VkDevice device);

    VkPipeline Get() const { return pipeline; }
    VkPipelineLayout GetLayout() const { return pipelineLayout; }

private:
    VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);
    std::vector<char> ReadFile(const std::string& filename);

    VkPipeline pipeline = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
};
