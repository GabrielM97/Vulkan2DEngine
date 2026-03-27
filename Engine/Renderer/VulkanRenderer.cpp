#include "VulkanRenderer.h"

#include <cassert>
#include <stdexcept>

#include "VulkanVertexBuffer.h"

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

void VulkanRenderer::Init(GLFWwindow* window, int width, int height)
{
    context.Init(window);

    device.Init(context);

    swapchain.Init(device, context.GetSurface(), width, height);

    m_ImagesInFlight.resize(swapchain.GetImageViews().size(), VK_NULL_HANDLE);

    renderPass.Init(device.GetDevice(), swapchain.GetFormat());

    m_Pipeline.Init(device.GetDevice(), swapchain.GetExtent(), renderPass.Get());

    m_Framebuffer.Init(device.GetDevice(), renderPass.Get(), swapchain.GetImageViews(), swapchain.GetExtent());

    m_CommandBuffer.Init(device.GetDevice(), device.GetGraphicsQueueFamily(), swapchain.GetImageViews().size());

    m_Sync.Init(device.GetDevice(), MAX_FRAMES_IN_FLIGHT);

    // Init per-image render finished semaphores
    m_RenderFinishedPerImage.resize(swapchain.GetImageViews().size());
    for (auto& sem : m_RenderFinishedPerImage)
    {
        VkSemaphoreCreateInfo semInfo{};
        semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(device.GetDevice(), &semInfo, nullptr, &sem);
    }

    // Track which images are currently in flight
    m_ImagesInFlight.resize(swapchain.GetImageViews().size(), VK_NULL_HANDLE);

    std::vector<Vertex> triangleVertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}
    };

    m_VertexBuffer.Init(device, m_CommandBuffer.GetCommandPool(), device.GetGraphicsQueue(), triangleVertices);
}

void VulkanRenderer::Cleanup()
{
    //wait for GPU to finish all work
    vkDeviceWaitIdle(device.GetDevice());

    m_VertexBuffer.Cleanup(device.GetDevice());

    for (auto sem : m_RenderFinishedPerImage)
        vkDestroySemaphore(device.GetDevice(), sem, nullptr);

    m_RenderFinishedPerImage.clear();
    m_ImagesInFlight.clear();

    m_Sync.Cleanup(device.GetDevice());

    m_CommandBuffer.Cleanup(device.GetDevice());

    m_Framebuffer.Cleanup(device.GetDevice());

    m_Pipeline.Cleanup(device.GetDevice());

    renderPass.Cleanup(device.GetDevice());

    swapchain.Cleanup(device.GetDevice());

    device.Cleanup();

    context.Cleanup();
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

void VulkanRenderer::DrawFrame()
{
    vkWaitForFences(device.GetDevice(), 1, &m_Sync.GetInFlightFence(m_CurrentFrame), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(device.GetDevice(), swapchain.Get(), UINT64_MAX,
                          m_Sync.GetImageAvailable(m_CurrentFrame), VK_NULL_HANDLE, &imageIndex);

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

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {m_Sync.GetImageAvailable(m_CurrentFrame)};
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    vkResetCommandBuffer(m_CommandBuffer.Get()[imageIndex], 0);

    RecordCommandBuffer(m_CommandBuffer.Get()[imageIndex], imageIndex);

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

    vkQueuePresentKHR(device.GetPresentQueue(), &presentInfo);
    m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
