#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"

#include "./vulkan.h"
#include "pong/memory.h"

static void pong_destroy_vk_device(void *null) {
    vkDestroyDevice(vulkan.device, NULL);
}

void pong_create_vk_device() {
    syslog(LOG_INFO, "info: creating vk device\n");

    VkDeviceCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueCreateInfoCount = (vulkan.queue.graphic_family == vulkan.queue.present_family ? 1 : 2),
        .pQueueCreateInfos = (VkDeviceQueueCreateInfo[]) {
            (VkDeviceQueueCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .queueFamilyIndex = vulkan.queue.graphic_family,
                .queueCount = 1,
                .pQueuePriorities = (float[]) { 1 },
            },
            (VkDeviceQueueCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .queueFamilyIndex = vulkan.queue.present_family,
                .queueCount = 1,
                .pQueuePriorities = (float[]) { 1 },
            },
        },
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = NULL,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = (const char *[]) {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        },
        .pEnabledFeatures = &(VkPhysicalDeviceFeatures) { 0 },
    };

    pong_vulkan_check(vkCreateDevice(vulkan.physical, &info, NULL, &vulkan.device));

    pong_add_cleanup(pong_destroy_vk_device, NULL);

    vkGetDeviceQueue(vulkan.device, vulkan.queue.graphic_family, 0, &vulkan.queue.graphic);
    vkGetDeviceQueue(vulkan.device, vulkan.queue.present_family, 0, &vulkan.queue.present);
}
