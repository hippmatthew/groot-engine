#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <GLFW/glfw3.h>

#include <map>

namespace ge
{

enum QueueFamilyType {
  Main = (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT),
  Async = (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT),
  Compute = VK_QUEUE_COMPUTE_BIT,
  Transfer = VK_QUEUE_TRANSFER_BIT,
  Sparse = VK_QUEUE_SPARSE_BINDING_BIT
};

struct QueueFamily {
  unsigned int index = -1;
  vk::raii::Queue vk_queue = nullptr;
};

class Context {
  friend class Allocator;

  using QueueFamilies = std::map<QueueFamilyType, QueueFamily>;

  public:
    Context();
    Context(Context&) = delete;
    Context(Context&&) = delete;

    ~Context();

    Context& operator = (Context&) = delete;
    Context& operator = (Context&&) = delete;

    void run() {
      run([](){});
    }

    template <typename Func>
    void run(Func&& code) {
      while (!shouldClose()) {
        pollEvents();
        code();
      }
      vk_device.waitIdle();
    }

    void close();

  private:
    static GLFWwindow * window();
    static const vk::raii::SurfaceKHR& surface();
    static const vk::raii::PhysicalDevice& physicalDevice();
    static const vk::raii::Device& device();
    static const unsigned int& queueIndex(QueueFamilyType);
    static const vk::raii::Queue& queue(QueueFamilyType);

    QueueFamilies getQueueFamilies(const vk::raii::PhysicalDevice&) const;
    unsigned int typeIndex(vk::PhysicalDeviceType) const;

    void createWindow();
    bool createInstance();
    void createSurface();
    void chooseGPU();
    void createDevice(bool);
    bool shouldClose();
    void pollEvents();

  private:
    static Context * p_context;

    GLFWwindow * gl_window = nullptr;

    vk::raii::Instance vk_instance = nullptr;
    vk::raii::SurfaceKHR vk_surface = nullptr;
    vk::raii::PhysicalDevice vk_physicalDevice = nullptr;
    vk::raii::Device vk_device = nullptr;
    QueueFamilies qfMap;

    bool closeRequested = false;
    std::vector<const char *> deviceExtensions = {
      VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};

} // namespace hlvl