#include <vulkan/vulkan.h>

#include <syslog.h>

#include "pong/vulkan.h"
#include "./vulkan.h"

#include "pong/memory.h"

void pong_create_vk_buffer(VkBuffer *buffer, unsigned size, VkBufferUsageFlags usage) {
    syslog(LOG_INFO, "info: creating vk buffer\n");

    VkBufferCreateInfo info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL,
    };

    pong_vulkan_check(vkCreateBuffer(vulkan.device, &info, NULL, buffer));
}

void pong_create_vk_memory(VkDeviceMemory *memory, VkBuffer buffer, VkMemoryPropertyFlags flags) {
    syslog(LOG_INFO, "info: allocating vk device memory\n");

    VkMemoryAllocateInfo info = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = NULL,
        .allocationSize = 0, // Later definition
        .memoryTypeIndex = -1, // Later definition
    };

    VkMemoryRequirements requirement;
    vkGetBufferMemoryRequirements(vulkan.device, buffer, &requirement);

    info.allocationSize = requirement.size;

    VkPhysicalDeviceMemoryProperties properties;
    vkGetPhysicalDeviceMemoryProperties(vulkan.physical, &properties);

    for (unsigned i = 0; i < properties.memoryTypeCount; ++i) {
        if (!(requirement.memoryTypeBits & (1 << i)))
            continue;

        if ((properties.memoryTypes[i].propertyFlags & flags) != flags)
            continue;

        info.memoryTypeIndex = i;

        break;
    }

    if (info.memoryTypeIndex == -1) {
        syslog(LOG_ERR, "error: failed to find suitable memory type\n");

        pong_finish();
    }

    pong_vulkan_check(vkAllocateMemory(vulkan.device, &info, NULL, memory));
}

void pong_map_vk_memory(void **mapped, VkBuffer buffer, VkDeviceMemory memory, unsigned size) {
    syslog(LOG_INFO, "info: mapping vk memory\n");

    pong_vulkan_check(vkBindBufferMemory(vulkan.device, buffer, memory, 0));

    pong_vulkan_check(vkMapMemory(vulkan.device, memory, 0, size, 0, mapped));
}
