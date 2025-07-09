#define VK_USE_PLATFORM_WAYLAND_KHR
#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"

#include "./window.h"
#include "pong/memory.h"

const char *const pong_vulkan_extensions[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
};

const unsigned pong_vulkan_extension_count = sizeof(pong_vulkan_extensions) / sizeof(char *);

// Don't add cleanup because it's vulkan module's responsability
void pong_vulkan_surface_create(VkInstance instance, VkSurfaceKHR *surface) {
    syslog(LOG_INFO, "info: creating vk surface\n");

    VkWaylandSurfaceCreateInfoKHR info = {
        .sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .display = wayland.display,
        .surface = wayland.surface,
    };

    pong_vulkan_check(vkCreateWaylandSurfaceKHR(instance, &info, NULL, surface));
}

void pong_vulkan_load_families(VkPhysicalDevice physical, unsigned *graphic, unsigned *present) {
    *graphic = -1;
    *present = -1;

    syslog(LOG_INFO, "info: loading vk queue families\n");

    unsigned count;

    vkGetPhysicalDeviceQueueFamilyProperties(physical, &count, NULL);

    VkQueueFamilyProperties properties[count];

    vkGetPhysicalDeviceQueueFamilyProperties(physical, &count, properties);

    for (unsigned i = 0; i < count && (*graphic == -1 || *present == -1); ++i) {
        if (*graphic == -1 && properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            *graphic = i;

        if (*present != -1 || vkGetPhysicalDeviceWaylandPresentationSupportKHR(physical, i, wayland.display) == VK_FALSE) continue;

        *present = i;
    }

    if (*graphic == -1 || *present == -1) {
        syslog(LOG_ERR, "error: couldn't find requested queues: graphic=%d, present=%d\n", *graphic, *present);

        pong_finish();
    }
}
