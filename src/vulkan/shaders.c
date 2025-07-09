#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"
#include "embed.h"

static void pong_destroy_vk_shader(VkShaderModule shader) {
    vkDestroyShaderModule(vulkan.device, shader, NULL);
}

void pong_create_vk_shaders() {
    syslog(LOG_INFO, "info: creating vk vertex shader\n");

    VkShaderModuleCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .codeSize = pong_embed_shaders_base_vert_spv_size,
        .pCode = (const unsigned *) pong_embed_shaders_base_vert_spv,
    };

    pong_vulkan_check(vkCreateShaderModule(vulkan.device, &info, NULL, &vulkan.pipeline.vertex));

    pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_shader, vulkan.pipeline.vertex);

    syslog(LOG_INFO, "info: creating vk fragment shader\n");

    info.codeSize = pong_embed_shaders_base_frag_spv_size,
    info.pCode = (const unsigned *) pong_embed_shaders_base_frag_spv,

    pong_vulkan_check(vkCreateShaderModule(vulkan.device, &info, NULL, &vulkan.pipeline.fragment));

    pong_add_cleanup((PONG_CLEANUP) pong_destroy_vk_shader, vulkan.pipeline.fragment);
}
