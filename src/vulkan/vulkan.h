#ifndef LOCAL_PONG_VULKAN_H
#define LOCAL_PONG_VULKAN_H

#include <vulkan/vulkan.h>

struct Vulkan {
    VkInstance instance;
    VkPhysicalDevice physical;
    VkDevice device;

    struct {
        unsigned graphic_family;
        unsigned present_family;
        VkQueue graphic;
        VkQueue present;
    } queue;

    struct {
        VkSurfaceKHR handle;
        VkSurfaceCapabilitiesKHR capabilities;
        VkSurfaceFormatKHR format;
        VkPresentModeKHR mode;

        VkExtent2D extent;
    } surface;

    struct {
        VkSwapchainKHR handle;
        VkImage *images;
        VkImageView *views;
        VkFramebuffer *framebuffers;
        unsigned image_count;
    } swapchain;

    struct {
        VkShaderModule vertex;
        VkShaderModule fragment;
        VkPipelineLayout layout;
        VkRenderPass render_pass;
        VkPipeline handle;
    } pipeline;

    struct {
        VkCommandPool pool;
        VkCommandBuffer draw;
    } command;

    struct {
        VkSemaphore image_available;
        VkSemaphore *render_finished;
        VkFence frame_pendant;
    } sync;

    struct {
        VkBuffer buffer;
        VkDeviceMemory memory;
        unsigned count;
    } objects;

    struct {
        VkBuffer buffer;
        VkDeviceMemory memory;
    } uniform;

    struct {
        VkDescriptorSetLayout layout;
        VkDescriptorPool pool;
        VkDescriptorSet set;
    } descriptor;
};

extern struct Vulkan vulkan;

void pong_create_vk_instance();
void pong_load_vk_physical();
void pong_create_vk_device();
void pong_load_vk_surface_data();
void pong_create_vk_swapchain();
void pong_create_vk_shaders();
void pong_create_vk_descriptor_layout();
void pong_create_vk_descriptor_pool();
void pong_allocate_vk_descriptor_set();
void pong_create_vk_pipeline_layout();
void pong_create_vk_render_pass();
void pong_create_vk_pipeline();
void pong_load_vk_images();
void pong_create_vk_views();
void pong_create_vk_framebuffers();
void pong_create_vk_command_pool();
void pong_create_vk_command_buffer();
void pong_create_vk_syncs();

void pong_create_vk_buffer(VkBuffer *buffer, unsigned size, VkBufferUsageFlags usage);
void pong_create_vk_memory(VkDeviceMemory *memory, VkBuffer buffer, VkMemoryPropertyFlags flags);
void pong_map_vk_memory(void **mapped, VkBuffer buffer, VkDeviceMemory memory, unsigned size);

void pong_record_commands(unsigned image_index);

#endif
