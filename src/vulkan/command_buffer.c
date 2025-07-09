#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

void pong_create_vk_command_buffer() {
    syslog(LOG_INFO, "info: creating vk command buffer\n");

    VkCommandBufferAllocateInfo info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = vulkan.command.pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    pong_vulkan_check(vkAllocateCommandBuffers(vulkan.device, &info, &vulkan.command.draw));
    
    // No cleanup, because vk command pool does it automatically
}

void pong_record_commands(unsigned image_index) {
    VkCommandBufferBeginInfo command_begin = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = 0,
        .pInheritanceInfo = NULL,
    };

    pong_vulkan_check(vkBeginCommandBuffer(vulkan.command.draw, &command_begin));

    VkRenderPassBeginInfo render_pass_begin = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = NULL,
        .renderPass = vulkan.pipeline.render_pass,
        .framebuffer = vulkan.swapchain.framebuffers[image_index],
        .renderArea = {
            .offset = {
                .x = 0,
                .y = 0,
            },
            
            .extent = vulkan.surface.extent,
        },

        .clearValueCount = 1,
        .pClearValues = (VkClearValue[]) {
            (VkClearValue) {
                .color = {
                    .float32 = { 0, 0, 0, 1 },
                },
            },
        },
    };

    vkCmdBeginRenderPass(vulkan.command.draw, &render_pass_begin, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(vulkan.command.draw, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan.pipeline.handle);

    vkCmdBindVertexBuffers(vulkan.command.draw, 0, 1, (VkBuffer[]) { vulkan.objects.buffer }, (VkDeviceSize[]) { 0 });

    vkCmdBindDescriptorSets(vulkan.command.draw, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan.pipeline.layout, 0, 1, (VkDescriptorSet[]) { vulkan.descriptor.set }, 0, NULL);

    vkCmdDraw(vulkan.command.draw, 6, vulkan.objects.count, 0, 0);

    vkCmdEndRenderPass(vulkan.command.draw);

    pong_vulkan_check(vkEndCommandBuffer(vulkan.command.draw));
}
