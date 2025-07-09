#include <vulkan/vulkan.h>

#include <stdlib.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"
#include "pong/pong.h"

void pong_load_vk_images() {
    syslog(LOG_INFO, "info: loading vk images\n");

    pong_vulkan_check(vkGetSwapchainImagesKHR(vulkan.device, vulkan.swapchain.handle, &vulkan.swapchain.image_count, NULL));

    vulkan.swapchain.images = malloc(vulkan.swapchain.image_count * sizeof(VkImage));

    pong_check(vulkan.swapchain.images != NULL);

    pong_add_cleanup(free, vulkan.swapchain.images);

    pong_vulkan_check(vkGetSwapchainImagesKHR(vulkan.device, vulkan.swapchain.handle, &vulkan.swapchain.image_count, vulkan.swapchain.images));
}
