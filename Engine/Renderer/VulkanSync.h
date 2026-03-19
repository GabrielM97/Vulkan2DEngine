#pragma once
#include <vector>
#include <vulkan/vulkan.h>

/**
 * @class VulkanSync
 * @brief Manages synchronization primitives for Vulkan rendering operations.
 *
 * VulkanSync is responsible for handling semaphores and fences used to
 * synchronize rendering operations between the GPU and the CPU. It provides
 * utilities to initialize and clean up synchronization objects and exposes
 * methods to retrieve specific synchronization primitives for frame-specific
 * operations.
 */
class VulkanSync
{
    /**
     * @brief Initializes the synchronization primitives for rendering frames.
     *
     * This method creates semaphores and fences necessary for synchronizing the rendering
     * workflow, such as image-available semaphores for presentation and in-flight fences
     * to manage GPU-CPU synchronization for each frame in flight.
     *
     * @param device The Vulkan logical device used to create the synchronization objects.
     * @param framesInFlight The number of frames that can be processed simultaneously.
     */
public:
    void Init(VkDevice device, int framesInFlight);
    /**
     * @brief Cleans up Vulkan synchronization objects associated with a device.
     *
     * This method iterates through the semaphores and fences maintained by the
     * VulkanSync class, destroys them using Vulkan API calls, and clears the
     * associated containers.
     *
     * @param device The Vulkan logical device used to destroy synchronization objects.
     */
    void Cleanup(VkDevice device);

    /**
     * Retrieves the semaphore associated with the specified frame index
     * that signals the availability of an image for rendering.
     *
     * @param currentFrame The index of the current frame in the flight sequence.
     * @return The Vulkan semaphore indicating image availability for the specified frame.
     */
    VkSemaphore GetImageAvailable(uint32_t currentFrame) const { return m_ImageAvailableSemaphores[currentFrame]; }
    /**
     * Retrieves the Vulkan fence associated with the current frame.
     *
     * This fence is used to synchronize the rendering operations for the specified frame.
     * It ensures that the GPU completes the processing of the current frame before moving to the next one.
     *
     * @param currentFrame The index of the current frame for which the fence is being retrieved.
     * @return A reference to the Vulkan fence associated with the specified frame.
     */
    VkFence& GetInFlightFence(uint32_t currentFrame) { return m_InFlightFences[currentFrame]; }

    /**
     * @brief A collection of Vulkan semaphores used to signal the availability of images for rendering.
     *
     * Each semaphore in this vector corresponds to a specific frame in flight and is signaled when an image
     * is available for rendering. These semaphores are utilized in synchronization to ensure proper
     * coordination between GPU operations, such as acquiring a swapchain image and submitting a draw
     * command buffer for rendering.
     *
     * The size of the vector is determined by the number of frames in flight, defined during initialization.
     * The semaphores are created during the initialization process and cleaned up when no longer needed.
     *
     * @note Managed internally by the VulkanSync class. Proper initialization and cleanup are required to
     * prevent resource leaks.
     */
private:
    std::vector<VkSemaphore> m_ImageAvailableSemaphores;
    /**
     * @brief Stores Vulkan fence objects used for synchronizing the GPU's work for frames in flight.
     *
     * This vector holds a VkFence for each frame in flight, where fences are used to ensure that
     * command buffers have completed execution before they are reused. Each frame's fence will
     * transition between a signaled and unsignaled state to coordinate the rendering across
     * multiple frames.
     *
     * Fences in this vector are created during the initialization of the Vulkan synchronization
     * primitives and are destroyed during cleanup. They are typically created in a signaled
     * state and reset as needed during the rendering process to manage GPU-CPU synchronization.
     */
    std::vector<VkFence> m_InFlightFences;
};
