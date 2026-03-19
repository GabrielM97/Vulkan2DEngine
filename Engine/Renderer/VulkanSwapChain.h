#pragma once
#include <vector>
#include <vulkan/vulkan.h>

class VulkanDevice;


class VulkanSwapChain
{
public:
    void Init(VulkanDevice& device, VkSurfaceKHR surface, int width, int height);


    void Cleanup(VkDevice device);


    const std::vector<VkImageView>& GetImageViews() const { return imageViews; }


    VkFormat GetFormat() const { return imageFormat; }


    VkExtent2D GetExtent() const { return extent; }


    VkSwapchainKHR& Get() { return swapchain; }

private:
    void CreateSwapchain(VulkanDevice& device, VkSurfaceKHR surface, int width, int height);


    void CreateImageViews(VkDevice device);


    VkSwapchainKHR swapchain = VK_NULL_HANDLE;


    std::vector<VkImage> images;


    std::vector<VkImageView> imageViews;

    VkFormat imageFormat;
    VkExtent2D extent;
};
