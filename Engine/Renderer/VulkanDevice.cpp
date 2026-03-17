#include "VulkanDevice.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "VulkanContext.h"

/**
 * @brief Initializes the Vulkan device by setting up the necessary Vulkan objects.
 *
 * This function is responsible for initializing the Vulkan device by performing the
 * following steps:
 * - Retrieving the Vulkan surface from the context.
 * - Selecting a suitable physical device for Vulkan operations.
 * - Creating the logical device to interface with the selected physical device.
 *
 * @param context The VulkanContext instance used to initialize the device. This
 * provides access to required Vulkan handles such as surface and instance.
 */
void VulkanDevice::Init(VulkanContext& context)
{
    surface = context.GetSurface();
    PickPhysicalDevice(context.GetInstance(), surface);
    CreateLogicalDevice();
}

/**
 * Cleans up the Vulkan logical device.
 *
 * If the Vulkan logical device (`device`) has been initialized (i.e., it is not set
 * to `VK_NULL_HANDLE`), this function destroys the logical device using the
 * Vulkan API call `vkDestroyDevice`.
 *
 * It is the responsibility of the caller to ensure that all resources
 * associated with the device (e.g., command buffers, pipelines) have been
 * properly released before calling this function.
 *
 * This function does not return any value and has no parameters.
 */
void VulkanDevice::Cleanup()
{
    if (device != VK_NULL_HANDLE)
    {
        vkDestroyDevice(device, nullptr);
    }
}

/**
 * Selects an appropriate Vulkan physical device (GPU) that is compatible with the specified instance
 * and surface. The selection is based on support for both graphics and presentation capabilities.
 *
 * @param instance The Vulkan instance used to retrieve physical devices.
 * @param surface The Vulkan surface used to check presentation support for the physical devices.
 *
 * @throws std::runtime_error If no Vulkan-compatible GPU is found or no suitable GPU supports
 * graphics and presentation capabilities.
 *
 * This function enumerates all available physical devices and checks each device for:
 * 1. Support for graphics operations via queue families with VK_QUEUE_GRAPHICS_BIT.
 * 2. Presentation support via vkGetPhysicalDeviceSurfaceSupportKHR.
 *
 * The first physical device that fulfills both criteria is selected and assigned to the
 * `physicalDevice` member of the class. The graphics and presentation queue family indices are
 * recorded as well.
 */
void VulkanDevice::PickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("No Vulkan-compatible GPUs found");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& dev : devices)
    {
        // Check if device supports graphics and presentation
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, queueFamilies.data());

        bool graphicsFound = false;
        bool presentFound = false;

        for (uint32_t i = 0; i < queueFamilies.size(); i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                graphicsQueueFamilyIndex = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, surface, &presentSupport);
            if (presentSupport)
                presentQueueFamilyIndex = i;

            if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && presentSupport)
            {
                graphicsFound = true;
                presentFound = true;
                break;
            }
        }

        if (graphicsFound && presentFound)
        {
            physicalDevice = dev;
            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(physicalDevice, &props);
            std::cout << "Selected a suitable GPU:" << props.deviceName << "\n";
            return;
        }
    }

    throw std::runtime_error("Failed to find a suitable GPU");
}

/**
 * @brief Creates a Vulkan logical device and initializes the necessary queues.
 *
 * This function creates a Vulkan logical device using the selected physical device.
 * It sets up the required queue creation information for both graphics and present operations,
 * enables necessary device features, and specifies validation layers and extensions if needed.
 * Upon success, the logical device and its associated queues are initialized.
 *
 * @throws std::runtime_error Thrown if the Vulkan logical device creation fails.
 */
void VulkanDevice::CreateLogicalDevice()
{
  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;

    // Graphics queue
    VkDeviceQueueCreateInfo graphicsQueueCreateInfo{};
    graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicsQueueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    graphicsQueueCreateInfo.queueCount = 1;
    graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(graphicsQueueCreateInfo);

    // If present queue is different, add it
    if (presentQueueFamilyIndex != graphicsQueueFamilyIndex)
    {
        VkDeviceQueueCreateInfo presentQueueCreateInfo{};
        presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        presentQueueCreateInfo.queueFamilyIndex = presentQueueFamilyIndex;
        presentQueueCreateInfo.queueCount = 1;
        presentQueueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(presentQueueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE; // useful later for textures

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;

#ifndef NDEBUG
    const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;
#else
    createInfo.enabledLayerCount = 0;
#endif

    // Extensions (for swapchain)
    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = deviceExtensions;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        throw std::runtime_error("Failed to create logical device");

    vkGetDeviceQueue(device, graphicsQueueFamilyIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(device, presentQueueFamilyIndex, 0, &presentQueue);

    std::cout << "Logical device and queues created successfully\n";
}
