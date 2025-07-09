#ifndef PONG_VULKAN_H
#define PONG_VULKAN_H

#include <vulkan/vulkan.h>

#include "pong/math.h"

struct PongVkObject {
    fvec2 position;
    fvec2 size;
};

extern const char *const pong_vulkan_extensions[];
extern const unsigned pong_vulkan_extension_count;

void pong_vulkan_create();
void pong_vulkan_create_objects(struct PongVkObject **objects, unsigned count);
void pong_vulkan_setup_wait_idle();

void pong_vulkan_draw();

void pong_vulkan_check(VkResult result);

void pong_vulkan_surface_create(VkInstance instance, VkSurfaceKHR *surface);
void pong_vulkan_load_families(VkPhysicalDevice physical, unsigned *graphic, unsigned *present);

unsigned pong_vulkan_get_width();
unsigned pong_vulkan_get_height();

#endif
