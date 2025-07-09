#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"

static void pong_destroy_vk_instance(void *null) {
    vkDestroyInstance(vulkan.instance, NULL);
}

void pong_create_vk_instance() {
    syslog(LOG_INFO, "info: creating vk instance\n");

    VkInstanceCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pApplicationInfo = &(VkApplicationInfo) {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = NULL,
            .pApplicationName = "pong",
            .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
            .pEngineName = NULL,
            .engineVersion = 0,
            .apiVersion = VK_API_VERSION_1_0,
        },
        .enabledLayerCount = 1,
        .ppEnabledLayerNames = (const char *[]) {
            "VK_LAYER_KHRONOS_validation",
        },
        .enabledExtensionCount = pong_vulkan_extension_count,
        .ppEnabledExtensionNames = pong_vulkan_extensions,
    };

    pong_vulkan_check(vkCreateInstance(&info, NULL, &vulkan.instance));

    pong_add_cleanup(pong_destroy_vk_instance, NULL);
}
