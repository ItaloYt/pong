#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"

#include "./vulkan.h"

#include "pong/memory.h"

static void pong_destroy_vk_swapchain(void *null) {
    vkDestroySwapchainKHR(vulkan.device, vulkan.swapchain.handle, NULL);
}

// We won't create new swapchains since we won't allow resizing
void pong_create_vk_swapchain() {
    syslog(LOG_INFO, "info: creating vk swapchain\n");

    VkSwapchainCreateInfoKHR info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .surface = vulkan.surface.handle,
        .minImageCount = vulkan.surface.capabilities.minImageCount,
        .imageFormat = vulkan.surface.format.format,
        .imageColorSpace = vulkan.surface.format.colorSpace,
        .imageExtent = vulkan.surface.extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE, // Later definition
        .queueFamilyIndexCount = 0, // Later definition
        .pQueueFamilyIndices = (const unsigned[]) {
            vulkan.queue.graphic_family,
            vulkan.queue.present_family,
        },
        .preTransform = vulkan.surface.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = vulkan.surface.mode,
        .clipped = VK_TRUE,
        .oldSwapchain = NULL,
    };

    if (vulkan.queue.graphic_family != vulkan.queue.present_family) {
        info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        info.queueFamilyIndexCount = 2;
    }

    pong_vulkan_check(vkCreateSwapchainKHR(vulkan.device, &info, NULL, &vulkan.swapchain.handle));

    pong_add_cleanup(pong_destroy_vk_swapchain, NULL);
}
