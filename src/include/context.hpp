#pragma once

#include "src/include/renderer.hpp"
#include "vulkan/vulkan_core.h"

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
  friend class Material;
  friend class Objects;
  friend class Renderer;

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
      load();

      while (!shouldClose()) {
        pollEvents();
        code();
        renderer.render();
      }

      vk_device.waitIdle();
    }

    void close();

    #ifdef ge_tests

      const GLFWwindow * get_window() const { return gl_window; }
      const vk::raii::Instance& get_instance() const { return vk_instance; }
      const vk::raii::PhysicalDevice& get_gpu() const { return vk_physicalDevice; }
      const vk::raii::Device& get_device() const { return vk_device; }
      const QueueFamilies& get_queues() const { return qfMap; }

    #endif // ge_tests

  private:
    static GLFWwindow * window();
    static const vk::raii::SurfaceKHR& surface();
    static const vk::raii::PhysicalDevice& physicalDevice();
    static const vk::raii::Device& device();
    static const unsigned int& queueIndex(QueueFamilyType);
    static const vk::raii::Queue& queue(QueueFamilyType);

    QueueFamilies getQueueFamilies(const vk::raii::PhysicalDevice&) const;
    unsigned int typeIndex(vk::PhysicalDeviceType) const;
    void load() const;

    void createWindow();
    bool createInstance();
    void createSurface();
    void chooseGPU(const std::vector<const char *>&);
    void createDevice(const bool&, std::vector<const char *>&);
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

    Renderer renderer;

    bool closeRequested = false;
};

} // namespace hlvl