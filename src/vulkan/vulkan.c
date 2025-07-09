#include <vulkan/vulkan.h>

#include <string.h>
#include <stdbool.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"

struct Vulkan vulkan;

static void pong_destroy_vk_surface(void *null) {
    vkDestroySurfaceKHR(vulkan.instance, vulkan.surface.handle, NULL);
}

static void pong_destroy_vk_buffer(VkBuffer buffer) {
    vkDestroyBuffer(vulkan.device, buffer, NULL);
}

static void pong_destroy_vk_memory(VkDeviceMemory memory) {
    vkFreeMemory(vulkan.device, memory, NULL);
}

static void pong_unmap_vk_memory(VkDeviceMemory memory) {
    vkUnmapMemory(vulkan.device, memory);
}

void pong_vulkan_create() {
    pong_create_vk_instance();

    pong_vulkan_surface_create(vulkan.instance, &vulkan.surface.handle);
    pong_add_cleanup(pong_destroy_vk_surface, NULL);

    pong_load_vk_physical();
    pong_vulkan_load_families(vulkan.physical, &vulkan.queue.graphic_family, &vulkan.queue.present_family);
    pong_load_vk_surface_data();
    pong_create_vk_device();
    pong_create_vk_swapchain();
    pong_create_vk_shaders();
    pong_create_vk_descriptor_layout();
    pong_create_vk_descriptor_pool();
    pong_allocate_vk_descriptor_set();
    pong_create_vk_pipeline_layout();
    pong_create_vk_render_pass();
    pong_create_vk_pipeline();
    pong_load_vk_images();
    pong_create_vk_views();
    pong_create_vk_framebuffers();
    pong_create_vk_command_pool();
    pong_create_vk_command_buffer();
    pong_create_vk_syncs();

    pong_create_vk_buffer(&vulkan.uniform.buffer, sizeof(fmat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_buffer, vulkan.uniform.buffer);

    pong_create_vk_memory(&vulkan.uniform.memory, vulkan.uniform.buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_memory, vulkan.uniform.memory);

    // Mapping uniform

    const fmat4 projection = {
        2.0 / vulkan.surface.extent.width, 0, 0, 0,
        0, 2.0 / vulkan.surface.extent.height, 0, 0,
        0, 0, 0, 0,
        -1, -1, 0, 1,
    };

    fmat4 *mapped;

    pong_map_vk_memory((void **) &mapped, vulkan.uniform.buffer, vulkan.uniform.memory, sizeof(fmat4));

    (void) memcpy(*mapped, projection, sizeof(fmat4));

    vkUnmapMemory(vulkan.device, vulkan.uniform.memory);

    // Update uniform
    VkWriteDescriptorSet writes[] = {
        (VkWriteDescriptorSet) {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = NULL,
            .dstSet = vulkan.descriptor.set,
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pImageInfo = NULL,
            .pBufferInfo = &(VkDescriptorBufferInfo) {
                .buffer = vulkan.uniform.buffer,
                .offset = 0,
                .range = sizeof(fmat4),
            },

            .pTexelBufferView = NULL,
        },
    };

    vkUpdateDescriptorSets(vulkan.device, 1, writes, 0, NULL);
}

void pong_vulkan_create_objects(struct PongVkObject **objects, unsigned count) {
    vulkan.objects.count = count;

    pong_create_vk_buffer(&vulkan.objects.buffer, count * sizeof(struct PongVkObject), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_buffer, vulkan.objects.buffer);

    pong_create_vk_memory(&vulkan.objects.memory, vulkan.objects.buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_memory, vulkan.objects.memory);

    pong_map_vk_memory((void **) objects, vulkan.objects.buffer, vulkan.objects.memory, count * sizeof(struct PongVkObject));
    pong_add_cleanup((PONG_CLEANUP) pong_unmap_vk_memory, vulkan.objects.memory);
}

unsigned pong_vulkan_get_width() {
    return vulkan.surface.extent.width;
}

unsigned pong_vulkan_get_height() {
    return vulkan.surface.extent.height;
}

static void pong_wait_vk_device(void *null) {
    (void) vkDeviceWaitIdle(vulkan.device);
}

void pong_vulkan_setup_wait_idle() {
    pong_add_cleanup(pong_wait_vk_device, NULL);
}

static bool pong_is_ready() {
    VkResult result = vkGetFenceStatus(vulkan.device, vulkan.sync.frame_pendant);

    if (result == VK_SUCCESS)
        return true;

    if (result == VK_NOT_READY)
        return false;

    pong_vulkan_check(result);

    return false;
}

void pong_vulkan_draw() {
    if (!pong_is_ready()) return;

    pong_vulkan_check(vkResetFences(vulkan.device, 1, &vulkan.sync.frame_pendant));

    unsigned image_index;
    pong_vulkan_check(vkAcquireNextImageKHR(vulkan.device, vulkan.swapchain.handle, -1, vulkan.sync.image_available, NULL, &image_index));

    pong_vulkan_check(vkResetCommandBuffer(vulkan.command.draw, 0));

    pong_record_commands(image_index);

    VkSubmitInfo submits[] = {
        (VkSubmitInfo) {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = NULL,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &vulkan.sync.image_available,
            .pWaitDstStageMask = (VkPipelineStageFlags[]) {
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            },
            
            .commandBufferCount = 1,
            .pCommandBuffers = (VkCommandBuffer[]) {
                vulkan.command.draw,
            },

            .signalSemaphoreCount = 1,
            .pSignalSemaphores = vulkan.sync.render_finished + image_index,
        }
    };

    pong_vulkan_check(vkQueueSubmit(vulkan.queue.graphic, 1, submits, vulkan.sync.frame_pendant));

    VkPresentInfoKHR present = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = NULL,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = vulkan.sync.render_finished + image_index,
        .swapchainCount = 1,
        .pSwapchains = (VkSwapchainKHR[]) {
            vulkan.swapchain.handle,
        },

        .pImageIndices = (unsigned[]) {
            image_index,
        },

        .pResults = NULL,
    };

    pong_vulkan_check(vkQueuePresentKHR(vulkan.queue.present, &present));
}

void pong_vulkan_check(VkResult result) {
    if (result == VK_SUCCESS) return;

    syslog(LOG_ERR, "error: result: %d\n", result);

    pong_finish();
}
