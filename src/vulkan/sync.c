#include <vulkan/vulkan.h>

#include <stdlib.h>
#include <stdbool.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"
#include "pong/pong.h"

static void pong_destroy_vk_semaphore(VkSemaphore semaphore) {
    vkDestroySemaphore(vulkan.device, semaphore, NULL);
}

static void pong_destroy_vk_fence(VkFence fence) {
    vkDestroyFence(vulkan.device, fence, NULL);
}

void pong_create_vk_syncs() {
    syslog(LOG_INFO, "info: creating vk sync objects\n");

    vulkan.sync.render_finished = malloc(vulkan.swapchain.image_count * sizeof(VkSemaphore));

    pong_check(vulkan.sync.render_finished != NULL);

    pong_add_cleanup(free, vulkan.sync.render_finished);

    VkSemaphoreCreateInfo semaphore = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
    };

    VkFenceCreateInfo fence = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = NULL,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    pong_vulkan_check(vkCreateSemaphore(vulkan.device, &semaphore, NULL, &vulkan.sync.image_available));
    pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_semaphore, vulkan.sync.image_available);

    pong_vulkan_check(vkCreateFence(vulkan.device, &fence, NULL, &vulkan.sync.frame_pendant));
    pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_fence, vulkan.sync.frame_pendant);

    for (unsigned i = 0; i < vulkan.swapchain.image_count; ++i) {
        pong_vulkan_check(vkCreateSemaphore(vulkan.device, &semaphore, NULL, vulkan.sync.render_finished + i));
        pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_semaphore, vulkan.sync.render_finished[i]);
    }
}
