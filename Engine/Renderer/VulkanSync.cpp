#include "VulkanSync.h"

#include <cassert>
#include <stdexcept>

void VulkanSync::Init(VkDevice device, int framesInFlight)
{
    m_ImageAvailableSemaphores.resize(framesInFlight);
    m_InFlightFences.resize(framesInFlight);

    VkSemaphoreCreateInfo semInfo{};
    semInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < framesInFlight; ++i)
    {
        vkCreateSemaphore(device, &semInfo, nullptr, &m_ImageAvailableSemaphores[i]);
        vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[i]);
    }
}

void VulkanSync::Cleanup(VkDevice device)
{
    for (auto sem : m_ImageAvailableSemaphores)
        vkDestroySemaphore(device, sem, nullptr);
    for (auto fence : m_InFlightFences)
        vkDestroyFence(device, fence, nullptr);

    m_ImageAvailableSemaphores.clear();
    m_InFlightFences.clear();
}
