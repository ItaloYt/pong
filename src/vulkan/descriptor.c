#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"

static void pong_destroy_vk_descriptor_layout(void *null) {
    vkDestroyDescriptorSetLayout(vulkan.device, vulkan.descriptor.layout, NULL);
}

void pong_create_vk_descriptor_layout() {
    syslog(LOG_INFO, "info: creating vk descriptor layout\n");

    VkDescriptorSetLayoutCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .bindingCount = 1,
        .pBindings = (VkDescriptorSetLayoutBinding[]) {
            (VkDescriptorSetLayoutBinding) {
                .binding = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
                .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                .pImmutableSamplers = NULL,
            },
        },
    };

    pong_vulkan_check(vkCreateDescriptorSetLayout(vulkan.device, &info, NULL, &vulkan.descriptor.layout));

    pong_add_cleanup(pong_destroy_vk_descriptor_layout, NULL);
}

static void pong_destroy_vk_descriptor_pool(void *null) {
    vkDestroyDescriptorPool(vulkan.device, vulkan.descriptor.pool, NULL);
}

void pong_create_vk_descriptor_pool() {
    syslog(LOG_INFO, "info: creating vk descriptor pool\n");

    VkDescriptorPoolCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .maxSets = 1,
        .poolSizeCount = 1,
        .pPoolSizes = (VkDescriptorPoolSize[]) {
            (VkDescriptorPoolSize) {
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
            },
        },
    };

    pong_vulkan_check(vkCreateDescriptorPool(vulkan.device, &info, NULL, &vulkan.descriptor.pool));

    pong_add_cleanup(pong_destroy_vk_descriptor_pool, NULL);
}

void pong_allocate_vk_descriptor_set() {
    syslog(LOG_INFO, "info: allocating vk descriptor set\n");

    VkDescriptorSetAllocateInfo info = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext = NULL,
        .descriptorPool = vulkan.descriptor.pool,
        .descriptorSetCount = 1,
        .pSetLayouts = (VkDescriptorSetLayout[]) {
            vulkan.descriptor.layout,
        },
    };

    pong_vulkan_check(vkAllocateDescriptorSets(vulkan.device, &info, &vulkan.descriptor.set));
}
