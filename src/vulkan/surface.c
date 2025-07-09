#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"

#include "./vulkan.h"
#include "pong/window.h"

#include "pong/math.h"

static void pong_load_vk_surface_capabilities();
static void pong_load_vk_surface_format();
static void pong_load_vk_surface_mode();

// We won't need resizing, so we won't run this multiple times
void pong_load_vk_surface_data() {
    pong_load_vk_surface_capabilities();
    pong_load_vk_surface_format();
    pong_load_vk_surface_mode();
}

static void pong_load_vk_surface_capabilities() {
    syslog(LOG_INFO, "info: loading vk surface capabilities\n");

    pong_vulkan_check(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan.physical, vulkan.surface.handle, &vulkan.surface.capabilities));

    ++vulkan.surface.capabilities.minImageCount;

    if (vulkan.surface.capabilities.maxImageCount != 0)
        vulkan.surface.capabilities.minImageCount = pong_math_imin(vulkan.surface.capabilities.minImageCount, vulkan.surface.capabilities.maxImageCount);

    vulkan.surface.extent = vulkan.surface.capabilities.currentExtent;

    if (vulkan.surface.extent.width != -1) return;

    vulkan.surface.extent.width = pong_math_iclamp(window.width, vulkan.surface.capabilities.minImageExtent.width, vulkan.surface.capabilities.maxImageExtent.width);
    vulkan.surface.extent.height = pong_math_iclamp(window.height, vulkan.surface.capabilities.minImageExtent.height, vulkan.surface.capabilities.maxImageExtent.height);
}

static void pong_load_vk_surface_format() {
    static const VkSurfaceFormatKHR desired = {
        .format = VK_FORMAT_B8G8R8A8_SRGB,
        .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
    };

    syslog(LOG_INFO, "info: loading vk surface format\n");

    unsigned count;

    pong_vulkan_check(vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan.physical, vulkan.surface.handle, &count, NULL));

    VkSurfaceFormatKHR formats[count];

    pong_vulkan_check(vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan.physical, vulkan.surface.handle, &count, formats));

    vulkan.surface.format = formats[0];

    for (unsigned i = 0; i < count; ++i) {
        if (formats[i].format == desired.format && formats[i].colorSpace == desired.colorSpace) {
            vulkan.surface.format = formats[i];

            break;
        }
    }
}

static void pong_load_vk_surface_mode() {
    static const VkPresentModeKHR desired = VK_PRESENT_MODE_MAILBOX_KHR;

    syslog(LOG_INFO, "info: loading vk surface mode\n");

    unsigned count = 0;

    pong_vulkan_check(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan.physical, vulkan.surface.handle, &count, NULL));

    VkPresentModeKHR modes[count];

    pong_vulkan_check(vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan.physical, vulkan.surface.handle, &count, modes));

    vulkan.surface.mode = VK_PRESENT_MODE_FIFO_KHR;

    for (unsigned i = 0; i < count; ++i) {
        if (modes[i] == desired) {
            vulkan.surface.mode = modes[i];

            break;
        }
    }
}
