#include "VulkanRenderer.h"

#include <cassert>
#include <stdexcept>

#include "VulkanVertexBuffer.h"

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

void VulkanRenderer::Init(GLFWwindow* window, int width, int height)
{
    m_Window = window;

    CreatePersistentResources(window);
    CreateSwapchainResources(width, height);

    // Hardcoded geometry for now.
    // Later this will move behind a higher-level 2D API.
    std::vector<Vertex> quadVertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}
    };

    m_VertexBuffer.Init(device, m_CommandBuffer.GetCommandPool(), device.GetGraphicsQueue(), quadVertices);
}

void VulkanRenderer::Cleanup()
{
    //wait for GPU to finish all work
    vkDeviceWaitIdle(device.GetDevice());

    m_VertexBuffer.Cleanup(device.GetDevice());

    DestroySwapchainResources();
    DestroyPersistentResources();
}

void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

    // Your render pass
    renderPassInfo.renderPass = renderPass.Get();

    // Framebuffer for THIS swapchain image
    renderPassInfo.framebuffer = m_Framebuffer.Get()[imageIndex];

    // Area to render to
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain.GetExtent();

    // Clear color (background)
    VkClearValue clearColor = {{{0.1f, 0.1f, 0.1f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    //Begin render pass
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Bind pipeline
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline.Get());

    //Bind vertex buffer
    VkBuffer vertexBuffers[] = {m_VertexBuffer.GetBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    assert(m_VertexBuffer.GetBuffer() != VK_NULL_HANDLE);
    //Draw
    vkCmdDraw(commandBuffer, m_VertexBuffer.GetVertexCount(), 1, 0, 0);

    // End render pass
    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer");
    }
}

void VulkanRenderer::CreatePersistentResources(GLFWwindow* window)
{
    context.Init(window);
    device.Init(context);

    // Frame-level sync objects survive swapchain recreation.
    m_Sync.Init(device.GetDevice(), MAX_FRAMES_IN_FLIGHT);
}

void VulkanRenderer::DestroyPersistentResources()
{
    m_Sync.Cleanup(device.GetDevice());
    device.Cleanup();
    context.Cleanup();
}

void VulkanRenderer::CreateSwapchainResources(int width, int height)
{
    swapchain.Init(device, context.GetSurface(), width, height);

    renderPass.Init(device.GetDevice(), swapchain.GetFormat());
    m_Pipeline.Init(device.GetDevice(), swapchain.GetExtent(), renderPass.Get());

    m_Framebuffer.Init(
        device.GetDevice(),
        renderPass.Get(),
        swapchain.GetImageViews(),
        swapchain.GetExtent()
    );

    m_CommandBuffer.Init(
        device.GetDevice(),
        device.GetGraphicsQueueFamily(),
        static_cast<uint32_t>(swapchain.GetImageViews().size())
    );

    CreatePerImageSyncObjects();

    m_CurrentFrame = 0;
}

void VulkanRenderer::DestroySwapchainResources()
{
    DestroyPerImageSyncObjects();
    m_CommandBuffer.Cleanup(device.GetDevice());
    m_Framebuffer.Cleanup(device.GetDevice());
    m_Pipeline.Cleanup(device.GetDevice());
    renderPass.Cleanup(device.GetDevice());
    swapchain.Cleanup(device.GetDevice());
 
}

void VulkanRenderer::CreatePerImageSyncObjects()
{
    // One tracking slot and one render-finished semaphore per swapchain image.
    m_ImagesInFlight.assign(swapchain.GetImageViews().size(), VK_NULL_HANDLE);

    // Start from a known-clean state in case this is called after recreation.
    m_RenderFinishedPerImage.clear();
    m_RenderFinishedPerImage.resize(swapchain.GetImageViews().size(), VK_NULL_HANDLE);

    for (auto& semaphore : m_RenderFinishedPerImage)
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(device.GetDevice(), &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS)
            throw std::runtime_error("Failed to create render-finished semaphore");
    }
}

void VulkanRenderer::DestroyPerImageSyncObjects()
{
    for (VkSemaphore semaphore : m_RenderFinishedPerImage)
    {
        if (semaphore != VK_NULL_HANDLE)
            vkDestroySemaphore(device.GetDevice(), semaphore, nullptr);
    }

    m_RenderFinishedPerImage.clear();
    m_ImagesInFlight.clear();
}

void VulkanRenderer::RecreateSwapchain()
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_Window, &width, &height);

    // A minimized window can report 0x0 framebuffer size.
    // Wait until the window is visible again before recreating.
    while (width == 0 || height == 0)
    {
        glfwWaitEvents();
        glfwGetFramebufferSize(m_Window, &width, &height);
    }
    
    RecreateSwapchain(width, height);
}

void VulkanRenderer::RecreateSwapchain(int width, int height)
{
    vkDeviceWaitIdle(device.GetDevice());

    DestroySwapchainResources();
    CreateSwapchainResources(width, height);
}

void VulkanRenderer::DrawFrame()
{
    vkWaitForFences(device.GetDevice(), 1, &m_Sync.GetInFlightFence(m_CurrentFrame), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult acquireResult  = vkAcquireNextImageKHR(device.GetDevice(), swapchain.Get(), UINT64_MAX,
                          m_Sync.GetImageAvailable(m_CurrentFrame), VK_NULL_HANDLE, &imageIndex);

    if (acquireResult == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapchain();
        return;
    }

    if (acquireResult != VK_SUCCESS && acquireResult != VK_SUBOPTIMAL_KHR)
        throw std::runtime_error("Failed to acquire swapchain image");
    
    // If this image is already being used, wait for it
    if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device.GetDevice(), 1,
                        &m_ImagesInFlight[imageIndex],
                        VK_TRUE,
                        UINT64_MAX);
    }

    m_ImagesInFlight[imageIndex] = m_Sync.GetInFlightFence(m_CurrentFrame);

    vkResetFences(device.GetDevice(), 1, &m_Sync.GetInFlightFence(m_CurrentFrame));
    vkResetCommandBuffer(m_CommandBuffer.Get()[imageIndex], 0);

    RecordCommandBuffer(m_CommandBuffer.Get()[imageIndex], imageIndex);

    VkSemaphore waitSemaphores[] = {m_Sync.GetImageAvailable(m_CurrentFrame)};
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_CommandBuffer.Get()[imageIndex];

    VkSemaphore signalSemaphores[] = {m_RenderFinishedPerImage[imageIndex]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkQueueSubmit(device.GetGraphicsQueue(), 1, &submitInfo, m_Sync.GetInFlightFence(m_CurrentFrame));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain.Get();
    presentInfo.pImageIndices = &imageIndex;

    VkResult presentResult = vkQueuePresentKHR(device.GetPresentQueue(), &presentInfo);

    if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
    {
        RecreateSwapchain();
        return;
    }

    m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanRenderer::OnFramebufferResized(int width, int height)
{
    // Ignore invalid framebuffer sizes.
    // This can happen while minimized.
    if (width == 0 || height == 0)
        return;

    RecreateSwapchain(width, height);
}
