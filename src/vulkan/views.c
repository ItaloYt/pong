#include <vulkan/vulkan.h>

#include <stdlib.h>
#include <stdbool.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"
#include "pong/pong.h"

static void pong_destroy_vk_view(VkImageView view) {
    vkDestroyImageView(vulkan.device, view, NULL);
}

// Since we won't allow resizing, we don't need to recreate the views
void pong_create_vk_views() {
    syslog(LOG_INFO, "info: creating vk image views\n");
    
    vulkan.swapchain.views = malloc(vulkan.swapchain.image_count * sizeof(VkImageView));

    pong_check(vulkan.swapchain.views != NULL);

    pong_add_cleanup(free, vulkan.swapchain.views);

    VkImageViewCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .image = NULL, // Later definition
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = vulkan.surface.format.format,
        .components = (VkComponentMapping) { VK_COMPONENT_SWIZZLE_IDENTITY },
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    for (unsigned i = 0; i < vulkan.swapchain.image_count; ++i) {
        info.image = vulkan.swapchain.images[i];

        pong_vulkan_check(vkCreateImageView(vulkan.device, &info, NULL, vulkan.swapchain.views + i));

        pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_view, vulkan.swapchain.views[i]);
    }
}
