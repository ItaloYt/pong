#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"

static void pong_destroy_vk_pipeline(void *null) {
    vkDestroyPipeline(vulkan.device, vulkan.pipeline.handle, NULL);
}

void pong_create_vk_pipeline() {
    syslog(LOG_INFO, "info: creating vk pipeline\n");

    VkGraphicsPipelineCreateInfo infos[] = {
        (VkGraphicsPipelineCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .stageCount = 2,
            .pStages = (VkPipelineShaderStageCreateInfo[]) {
                (VkPipelineShaderStageCreateInfo) {
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .pNext = NULL,
                    .flags = 0,
                    .stage = VK_SHADER_STAGE_VERTEX_BIT,
                    .module = vulkan.pipeline.vertex,
                    .pName = "main",
                    .pSpecializationInfo = NULL,
                },

                (VkPipelineShaderStageCreateInfo) {
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .pNext = NULL,
                    .flags = 0,
                    .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                    .module = vulkan.pipeline.fragment,
                    .pName = "main",
                    .pSpecializationInfo = NULL,
                },
            },

            .pVertexInputState = &(VkPipelineVertexInputStateCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .vertexBindingDescriptionCount = 1,
                .pVertexBindingDescriptions = (VkVertexInputBindingDescription[]) {
                    (VkVertexInputBindingDescription) {
                        .binding = 0,
                        .stride = sizeof(struct PongVkObject),
                        .inputRate = VK_VERTEX_INPUT_RATE_INSTANCE,
                    },
                },

                .vertexAttributeDescriptionCount = 2,
                .pVertexAttributeDescriptions = (VkVertexInputAttributeDescription[]) {
                    (VkVertexInputAttributeDescription) {
                        .location = 0,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32_SFLOAT,
                        .offset = 0,
                    },

                    (VkVertexInputAttributeDescription) {
                        .location = 1,
                        .binding = 0,
                        .format = VK_FORMAT_R32G32_SFLOAT,
                        .offset = sizeof(fvec2),
                    },
                },
            },

            .pInputAssemblyState = &(VkPipelineInputAssemblyStateCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                .primitiveRestartEnable = VK_FALSE,
            },

            .pTessellationState = &(VkPipelineTessellationStateCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .patchControlPoints = 0,
            },

            .pViewportState = &(VkPipelineViewportStateCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .viewportCount = 1,
                .pViewports = (VkViewport[]) {
                    (VkViewport) {
                        .x = 0,
                        .y = 0,
                        .width = vulkan.surface.extent.width,
                        .height = vulkan.surface.extent.height,
                        .minDepth = 0,
                        .maxDepth = 1,
                    },
                },

                .scissorCount = 1,
                .pScissors = (VkRect2D[]) {
                    (VkRect2D) {
                        .offset = {
                            .x = 0,
                            .y = 0
                        },
                        
                        .extent = vulkan.surface.extent,
                    },
                },
            },

            .pRasterizationState = &(VkPipelineRasterizationStateCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .depthClampEnable = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode = VK_POLYGON_MODE_FILL,
                .cullMode = VK_CULL_MODE_BACK_BIT,
                .frontFace = VK_FRONT_FACE_CLOCKWISE,
                .depthBiasEnable = VK_FALSE,
                .depthBiasConstantFactor = 0,
                .depthBiasClamp = 0,
                .depthBiasSlopeFactor = 0,
                .lineWidth = 1,
            },

            .pMultisampleState = &(VkPipelineMultisampleStateCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                .sampleShadingEnable = VK_FALSE,
                .minSampleShading = 0,
                .pSampleMask = NULL,
                .alphaToCoverageEnable = VK_FALSE,
                .alphaToOneEnable = VK_FALSE,
            },

            .pDepthStencilState = NULL,

            .pColorBlendState = &(VkPipelineColorBlendStateCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .logicOpEnable = VK_FALSE,
                .logicOp = VK_LOGIC_OP_COPY,
                .attachmentCount = 1,
                .pAttachments = (VkPipelineColorBlendAttachmentState[]) {
                    (VkPipelineColorBlendAttachmentState) {
                        .blendEnable = VK_FALSE,
                        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                        .colorBlendOp = VK_BLEND_OP_ADD,
                        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                        .alphaBlendOp = VK_BLEND_OP_ADD,
                        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
                    },
                },
                .blendConstants = { 0, 0, 0, 0 },
            },

            // We won't be resizing, so we don't need any dynamic state
            .pDynamicState = &(VkPipelineDynamicStateCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .pNext = NULL,
                .flags = 0,
                .dynamicStateCount = 0,
                .pDynamicStates = NULL,
            },

            .layout = vulkan.pipeline.layout,
            .renderPass = vulkan.pipeline.render_pass,
            .subpass = 0,
            .basePipelineHandle = NULL,
            .basePipelineIndex = -1,
        }
    };

    pong_vulkan_check(vkCreateGraphicsPipelines(vulkan.device, NULL, 1, infos, NULL, &vulkan.pipeline.handle));

    pong_add_cleanup(pong_destroy_vk_pipeline, NULL);
}
