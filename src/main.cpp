#include "VkBootstrap.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_vulkan.h>
#include <VkBootstrapDispatch.h>
#include <fmt/base.h>
#include <fmt/format.h>
#include <vulkan/vulkan_core.h>

namespace Graphics {
struct Init {
  SDL_Window *window;
  vkb::Instance instance;
  vkb::InstanceDispatchTable inst_disp;
  VkSurfaceKHR surface;
  vkb::Device device;
  vkb::DispatchTable disp;
  vkb::Swapchain swapchain;
};

struct RenderData {
  VkQueue graphics_queue;
  VkQueue present_queue;
  std::vector<VkImage> swapchain_images;
  std::vector<VkImageView> swapchain_image_views;
  std::vector<VkFramebuffer> framebuffers;

  VkRenderPass render_pass;
  VkPipelineLayout pipeline_layout;
  VkPipeline graphics_pipeline;

  VkCommandPool command_pool;
  std::vector<VkCommandBuffer> command_buffers;

  std::vector<VkSemaphore> available_semaphores;
  std::vector<VkSemaphore> finished_semaphore;
  std::vector<VkFence> in_flight_fences;
  std::vector<VkFence> image_in_flight;
  size_t current_frame = 0;
};
SDL_Window *get_window() {
  return SDL_CreateWindow("vk-001", 1280, 720,
                          SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN);
}
void DestroyWindow(SDL_Window *window) { SDL_DestroyWindow(window); }

VkSurfaceKHR create_surface(VkInstance instance, SDL_Window *window,
                            VkAllocationCallbacks *allocator = nullptr) {
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  bool err = SDL_Vulkan_CreateSurface(window, instance, allocator, &surface);
  if (err) {
    const char *error_msg = SDL_GetError();
    fmt::println("Error: {}", error_msg);
    surface = VK_NULL_HANDLE;
  }
  return surface;
}
} // namespace Graphics

int main(int argc, char *argv[]) {
  vkb::InstanceBuilder builder;
  auto inst_ret = builder.set_app_name("vk-001")
                      .request_validation_layers()
                      .use_default_debug_messenger()
                      .build();
  if (!inst_ret) { /* Assume error will go here */
  }
  vkb::Instance vkb_inst = inst_ret.value();
  vkb::PhysicalDeviceSelector selector{vkb_inst};
  auto window = Graphics::get_window();
  VkSurfaceKHR vk_surface = VK_NULL_HANDLE;
  auto surface =
      SDL_Vulkan_CreateSurface(window, vkb_inst, nullptr, &vk_surface);

  auto phys_ret = selector.set_surface(vk_surface)
                      .set_minimum_version(1, 1)
                      .require_dedicated_transfer_queue()
                      .select();
  if (!phys_ret) {
    /* We will think about it later */
  }

  vkb::DeviceBuilder device_builder{phys_ret.value()};
  auto dev_ret = device_builder.build();
  if (!dev_ret) { /* We will think about it later */
  }
  vkb::Device vkb_device = dev_ret.value();
  auto graphic_queue_ret = vkb_device.get_queue(vkb::QueueType::graphics);
  if (!graphic_queue_ret) {
  }

  VkQueue graphics_queue = graphic_queue_ret.value();
  return 0;
}
