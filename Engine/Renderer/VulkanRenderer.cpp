#include "VulkanRenderer.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Scene/SpriteRenderer.h>

#include "VulkanVertexBuffer.h"

class SpriteRenderer;
constexpr int MAX_FRAMES_IN_FLIGHT = 2;

void VulkanRenderer::Init(GLFWwindow* window, int width, int height)
{
    m_Window = window;

    m_FramebufferWidth = width;
    m_FramebufferHeight = height;

    CreatePersistentResources(window);
    CreateGlobalResources(width, height);
    CreateSwapchainResources(width, height);

    std::vector<Vertex> quadVertices = {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}
    };
    
    std::vector<uint32_t> quadIndices = {
        0, 1, 2,
        0, 2, 3
    };

    m_VertexBuffer.Init(device, m_CommandBuffer.GetCommandPool(), device.GetGraphicsQueue(), quadVertices);
    
    m_IndexBuffer.Init(device, m_CommandBuffer.GetCommandPool(), device.GetGraphicsQueue(), quadIndices);
    
    m_InstanceBuffer.Init(device, 1000);
}

void VulkanRenderer::Cleanup()
{
    vkDeviceWaitIdle(device.GetDevice());
    
    m_InstanceBuffer.Cleanup(device.GetDevice());
    m_IndexBuffer.Cleanup(device.GetDevice());
    m_VertexBuffer.Cleanup(device.GetDevice());

    DestroySwapchainResources();
    DestroyGlobalResources();
    DestroyPersistentResources();
}

void VulkanRenderer::BeginFrame()
{
    m_QuadCommands.clear();
}

uint32_t VulkanRenderer::LoadTexture(const char* path)
{
    VulkanTexture texture;
    texture.Init(
        device,
        m_UploadCommandPool,
        device.GetGraphicsQueue(),
        path
    );

    m_Textures.push_back(std::move(texture));

    const uint32_t textureIndex = static_cast<uint32_t>(m_Textures.size() - 1);
    
    RebuildTextureDescriptorResources();

    return textureIndex;
}

void VulkanRenderer::SetCamera(const Camera2D& camera) 
{
    m_Camera = camera;
    m_CameraDirty = true;
}

void VulkanRenderer::DrawQuad(const glm::vec2 position, const glm::vec2 size, float rotationDegrees, const glm::vec2 uvMin, const glm::vec2 uvMax, const glm::vec4 tint, uint32_t textureIndex)
{
    QuadCommand command{};
    command.position = position;
    command.size = size;
    command.rotation = glm::radians(rotationDegrees);
    command.uvMin = uvMin;
    command.uvMax = uvMax;
    command.tint = tint;
    command.textureIndex = textureIndex;

    m_QuadCommands.push_back(command);
}

void VulkanRenderer::EndFrame()
{
    m_InstanceData.clear();
    m_PreparedBatches.clear();

    m_InstanceData.reserve(m_QuadCommands.size());

    for (const QuadCommand& quad : m_QuadCommands)
    {
        if (quad.textureIndex >= m_TextureDescriptorSets.size())
            throw std::runtime_error("Quad uses invalid texture index");

        if (m_PreparedBatches.empty() || m_PreparedBatches.back().textureIndex != quad.textureIndex)
        {
            PreparedBatch prepared{};
            prepared.textureIndex = quad.textureIndex;
            prepared.firstInstance = static_cast<uint32_t>(m_InstanceData.size());
            m_PreparedBatches.push_back(prepared);
        }

        QuadInstanceData instance{};
        instance.position = quad.position;
        instance.size = quad.size;
        instance.rotation = quad.rotation;
        instance.textureIndex = static_cast<float>(quad.textureIndex);
        instance.uvMin = quad.uvMin;
        instance.uvMax = quad.uvMax;
        instance.tint = quad.tint;

        m_InstanceData.push_back(instance);
        m_PreparedBatches.back().instanceCount++;
    }

    m_InstanceBuffer.Update(device, m_InstanceData);

    DrawFrame();
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
    renderPassInfo.renderPass = m_renderPass.Get();

    // Framebuffer for THIS swapchain image
    renderPassInfo.framebuffer = m_Framebuffer.Get()[imageIndex];

    // Area to render to
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_swapchain.GetExtent();

    // Clear color (background)
    VkClearValue clearColor = {{{0.1f, 0.1f, 0.1f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    //Begin render pass
    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline.Get());

    VkBuffer vertexBuffers[] = {
        m_VertexBuffer.GetBuffer(),
        m_InstanceBuffer.GetBuffer()
    };

    VkDeviceSize offsets[] = {0, 0};

    vkCmdBindVertexBuffers(commandBuffer, 0, 2, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

    for (const PreparedBatch& batch : m_PreparedBatches)
    {
        VkDescriptorSet descriptorSet = m_TextureDescriptorSets[batch.textureIndex];

        vkCmdBindDescriptorSets(
            commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_Pipeline.GetLayout(),
            0,
            1,
            &descriptorSet,
            0,
            nullptr
        );

        vkCmdDrawIndexed(
            commandBuffer,
            m_IndexBuffer.GetIndexCount(),
            batch.instanceCount,
            0,
            0,
            batch.firstInstance
        );
    }
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

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    poolInfo.queueFamilyIndex = device.GetGraphicsQueueFamily();

    if (vkCreateCommandPool(device.GetDevice(), &poolInfo, nullptr, &m_UploadCommandPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create upload command pool");

    // Frame-level sync objects survive swapchain recreation.
    m_Sync.Init(device.GetDevice(), MAX_FRAMES_IN_FLIGHT);
}

void VulkanRenderer::DestroyPersistentResources()
{
    m_Sync.Cleanup(device.GetDevice());

    if (m_UploadCommandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(device.GetDevice(), m_UploadCommandPool, nullptr);
        m_UploadCommandPool = VK_NULL_HANDLE;
    }

    device.Cleanup();
    context.Cleanup();
}

void VulkanRenderer::CreateGlobalResources(int width, int height)
{
    CreateDescriptorSetLayout();

    m_GlobalUniformBuffer.Init(device, sizeof(GlobalUBO));
    
    CreateDescriptorPool();
    CreateDescriptorSets();

    m_CameraDirty = true;
}

void VulkanRenderer::DestroyGlobalResources()
{
    if (m_DescriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(device.GetDevice(), m_DescriptorPool, nullptr);
        m_DescriptorPool = VK_NULL_HANDLE;
    }

    m_TextureDescriptorSets.clear();

    m_GlobalUniformBuffer.Cleanup(device.GetDevice());

    for (auto& texture : m_Textures)
        texture.Cleanup(device.GetDevice());

    m_Textures.clear();
    m_TextureCache.clear();
    m_FallbackTextureIndex = 0;
    m_HasFallbackTexture = false;

    if (m_DescriptorSetLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(device.GetDevice(), m_DescriptorSetLayout, nullptr);
        m_DescriptorSetLayout = VK_NULL_HANDLE;
    }
}

void VulkanRenderer::CreateDescriptorSetLayout()
{
    VkDescriptorSetLayoutBinding uboBinding{};
    uboBinding.binding = 0;
    uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboBinding.descriptorCount = 1;
    uboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboBinding.pImmutableSamplers = nullptr;
    
    VkDescriptorSetLayoutBinding samplerBinding{};
    samplerBinding.binding = 1;
    samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerBinding.descriptorCount = 1;
    samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    samplerBinding.pImmutableSamplers = nullptr;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
        uboBinding,
        samplerBinding
    };

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 2;
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device.GetDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
        throw std::runtime_error("Failed to create descriptor set layout");
}

void VulkanRenderer::CreateDescriptorPool()
{
    const uint32_t textureCount = static_cast<uint32_t>(m_Textures.size());
    
    if (textureCount == 0)
        return;

    std::array<VkDescriptorPoolSize, 2> poolSizes{};

    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = textureCount;

    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = textureCount;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = textureCount;

    if (vkCreateDescriptorPool(device.GetDevice(), &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create descriptor pool");
}

void VulkanRenderer::CreateDescriptorSets()
{
   const uint32_t textureCount = static_cast<uint32_t>(m_Textures.size());
    
    if (textureCount == 0)
        return;

    std::vector<VkDescriptorSetLayout> layouts(textureCount, m_DescriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = textureCount;
    allocInfo.pSetLayouts = layouts.data();

    m_TextureDescriptorSets.resize(textureCount);

    if (vkAllocateDescriptorSets(device.GetDevice(), &allocInfo, m_TextureDescriptorSets.data()) != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate texture descriptor sets");

    for (uint32_t i = 0; i < textureCount; ++i)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = m_GlobalUniformBuffer.GetBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(GlobalUBO);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = m_Textures[i].GetImageView();
        imageInfo.sampler = m_Textures[i].GetSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = m_TextureDescriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = m_TextureDescriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(
            device.GetDevice(),
            static_cast<uint32_t>(descriptorWrites.size()),
            descriptorWrites.data(),
            0,
            nullptr
        );
    }
}

void VulkanRenderer::RebuildTextureDescriptorResources()
{
    vkDeviceWaitIdle(device.GetDevice());

    if (m_DescriptorPool != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(device.GetDevice(), m_DescriptorPool, nullptr);
        m_DescriptorPool = VK_NULL_HANDLE;
    }

    m_TextureDescriptorSets.clear();

    if (m_Textures.empty())
        return;

    CreateDescriptorPool();
    CreateDescriptorSets();
}

void VulkanRenderer::CreateFallbackTexture()
{
    if (m_HasFallbackTexture)
        return;

    // Temporary fallback: use a known-good project texture.
    // This can later be replaced with a dedicated missing-texture asset.
    m_FallbackTextureIndex = LoadTexture("Assets/Textures/texture.jpg");
    m_HasFallbackTexture = true;
}

void VulkanRenderer::UpdateCameraMatrices() 
{
    if (m_FramebufferWidth <= 0 || m_FramebufferHeight <= 0)
        return;

    GlobalUBO ubo{};

    const float zoom = glm::max(m_Camera.GetZoom(), 0.01f);

    const float viewWidth = static_cast<float>(m_FramebufferWidth) / zoom;
    const float viewHeight = static_cast<float>(m_FramebufferHeight) / zoom;

    const float left = m_Camera.GetPosition().x;
    const float right = m_Camera.GetPosition().x + viewWidth;
    const float top = m_Camera.GetPosition().y;
    const float bottom = m_Camera.GetPosition().y + viewHeight;

    ubo.viewProjection = glm::ortho(
        left,
        right,
        bottom,
        top,
        -1.0f,
        1.0f
    );

    ubo.viewProjection[1][1] *= -1.0f;
    ubo.viewProjection[3][1] *= -1.0f;

    m_GlobalUniformBuffer.Update(
        device.GetDevice(),
        &ubo,
        sizeof(GlobalUBO)
    );
}

void VulkanRenderer::CreateSwapchainResources(int width, int height)
{
    m_swapchain.Init(device, context.GetSurface(), width, height);

    m_renderPass.Init(device.GetDevice(), m_swapchain.GetFormat());
    m_Pipeline.Init(device.GetDevice(), m_swapchain.GetExtent(), m_renderPass.Get(), m_DescriptorSetLayout);

    m_Framebuffer.Init(
        device.GetDevice(),
        m_renderPass.Get(),
        m_swapchain.GetImageViews(),
        m_swapchain.GetExtent()
    );

    m_CommandBuffer.Init(
        device.GetDevice(),
        device.GetGraphicsQueueFamily(),
        static_cast<uint32_t>(m_swapchain.GetImageViews().size())
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
    m_renderPass.Cleanup(device.GetDevice());
    m_swapchain.Cleanup(device.GetDevice());
}

void VulkanRenderer::CreatePerImageSyncObjects()
{
    // One tracking slot and one render-finished semaphore per swapchain image.
    m_ImagesInFlight.assign(m_swapchain.GetImageViews().size(), VK_NULL_HANDLE);

    // Start from a known-clean state in case this is called after recreation.
    m_RenderFinishedPerImage.clear();
    m_RenderFinishedPerImage.resize(m_swapchain.GetImageViews().size(), VK_NULL_HANDLE);

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

void VulkanRenderer::DrawSprite(const Transform2D& transform, const SpriteRenderer& sprite)
{
    uint32_t textureIndex = GetOrLoadTexture(sprite.GetTexture().path);

    glm::vec2 uvMin{0.0f, 0.0f};
    glm::vec2 uvMax{1.0f, 1.0f};

    if (sprite.UsesSourceRect())
    {
        if (const VulkanTexture* texture = GetTexture(textureIndex))
        {
            const float textureWidth = static_cast<float>(texture->GetWidth());
            const float textureHeight = static_cast<float>(texture->GetHeight());

            if (textureWidth > 0.0f && textureHeight > 0.0f)
            {
                uvMin.x = static_cast<float>(sprite.GetSourceRect().x) / textureWidth;
                uvMin.y = static_cast<float>(sprite.GetSourceRect().y) / textureHeight;

                uvMax.x = static_cast<float>(sprite.GetSourceRect().x + sprite.GetSourceRect().width) / textureWidth;
                uvMax.y = static_cast<float>(sprite.GetSourceRect().y + sprite.GetSourceRect().height) / textureHeight;
            }
        }
    }

    DrawQuad(
        transform.position,
        transform.size,
        transform.rotationDegrees,
        uvMin,
        uvMax,
        sprite.GetTint(),
        textureIndex
    );
}

uint32_t VulkanRenderer::GetOrLoadTexture(const std::string& path)
{
    auto it = m_TextureCache.find(path);
    if (it != m_TextureCache.end())
        return it->second;

    try
    {
        uint32_t index = LoadTexture(path.c_str());
        m_TextureCache[path] = index;
        return index;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "Failed to load texture '" << path
                  << "'. Using fallback texture instead. Reason: "
                  << exception.what() << "\n";

        if (!m_HasFallbackTexture)
            CreateFallbackTexture();

        m_TextureCache[path] = m_FallbackTextureIndex;
        return m_FallbackTextureIndex;
    }
}

const VulkanTexture* VulkanRenderer::GetTexture(uint32_t index) const
{
    if (index >= m_Textures.size())
        return nullptr;

    return &m_Textures[index];
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

    m_FramebufferWidth = width;
    m_FramebufferHeight = height;

    DestroySwapchainResources();
    CreateSwapchainResources(width, height);

    m_CameraDirty = true;
}

void VulkanRenderer::DrawFrame()
{
    vkWaitForFences(device.GetDevice(), 1, &m_Sync.GetInFlightFence(m_CurrentFrame), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult acquireResult  = vkAcquireNextImageKHR(device.GetDevice(), m_swapchain.Get(), UINT64_MAX,
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

    if (m_CameraDirty)
    {
        UpdateCameraMatrices();
        m_CameraDirty = false;
    }

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
    presentInfo.pSwapchains = &m_swapchain.Get();
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
