#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"

static void pong_destroy_vk_command_pool(void *null) {
    vkDestroyCommandPool(vulkan.device, vulkan.command.pool, NULL);
}

void pong_create_vk_command_pool() {
    syslog(LOG_INFO, "info: creating vk command pool\n");

    VkCommandPoolCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = vulkan.queue.graphic_family,
    };

    pong_vulkan_check(vkCreateCommandPool(vulkan.device, &info, NULL, &vulkan.command.pool));

    pong_add_cleanup(pong_destroy_vk_command_pool, NULL);
}
