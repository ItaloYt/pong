#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"

#include "./vulkan.h"

void pong_load_vk_physical() {
    syslog(LOG_INFO, "info: selecting physical device\n");

    unsigned count = 0;

    pong_vulkan_check(vkEnumeratePhysicalDevices(vulkan.instance, &count, NULL));

    VkPhysicalDevice devices[count];

    pong_vulkan_check(vkEnumeratePhysicalDevices(vulkan.instance, &count, devices));

    // We won't be using any special features, so we don't need a specific device
    vulkan.physical = devices[0];
}
