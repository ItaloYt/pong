#include <vulkan/vulkan.h>

#include <stdlib.h>
#include <stdbool.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"
#include "pong/pong.h"

static void pong_destroy_vk_framebuffer(VkFramebuffer framebuffer) {
    vkDestroyFramebuffer(vulkan.device, framebuffer, NULL);
}

// We won't allow resizing, so we don't need to recreate the framebuffers
void pong_create_vk_framebuffers() {
    syslog(LOG_INFO, "info: creating vk framebuffers\n");

    vulkan.swapchain.framebuffers = malloc(vulkan.swapchain.image_count * sizeof(VkFramebuffer));

    pong_check(vulkan.swapchain.framebuffers != NULL);

    pong_add_cleanup(free, vulkan.swapchain.framebuffers);

    VkFramebufferCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .renderPass = vulkan.pipeline.render_pass,
        .attachmentCount = 1,
        .pAttachments = NULL, // Later definition
        .width = vulkan.surface.extent.width,
        .height = vulkan.surface.extent.height,
        .layers = 1,
    };

    for (unsigned i = 0; i < vulkan.swapchain.image_count; ++i) {
        info.pAttachments = vulkan.swapchain.views + i;

        pong_vulkan_check(vkCreateFramebuffer(vulkan.device, &info, NULL, vulkan.swapchain.framebuffers + i));

        pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_framebuffer, vulkan.swapchain.framebuffers[i]);
    }
}
