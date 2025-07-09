#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"

static void pong_destroy_vk_pipeline_layout(void *null) {
    vkDestroyPipelineLayout(vulkan.device, vulkan.pipeline.layout, NULL);
}

void pong_create_vk_pipeline_layout() {
    syslog(LOG_INFO, "info: creating vk pipeline layout\n");

    VkPipelineLayoutCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .setLayoutCount = 1,
        .pSetLayouts = (VkDescriptorSetLayout[]) {
            vulkan.descriptor.layout,
        },

        .pushConstantRangeCount = 0,
        .pPushConstantRanges = NULL,
    };

    pong_vulkan_check(vkCreatePipelineLayout(vulkan.device, &info, NULL, &vulkan.pipeline.layout));

    pong_add_cleanup(pong_destroy_vk_pipeline_layout, NULL);
}
