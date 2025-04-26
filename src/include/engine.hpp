#pragma once

#include "src/include/materials.hpp"
#include "src/include/vkcontext.hpp"

#include <GLFW/glfw3.h>

#include <string>

#define ge_make_version(major, minor, patch) (unsigned int)( \
  ( (major) << 24 ) | \
  ( (minor) << 16 ) | \
  (patch) \
)

namespace ge {

enum BufferMode {
  SingleBuffer = 1,
  DoubleBuffer = 2,
  TripleBuffer = 3
};

struct Settings {
  unsigned int gpu_index = 0;
  unsigned int window_width = 1280;
  unsigned int window_height = 720;
  std::string window_title = "Groot Engine";
  std::string application_name = "Groot Engine Application";
  unsigned int application_version = ge_make_version(1, 0, 0);
  BufferMode buffer_mode = TripleBuffer;
  vk::Format format = vk::Format::eB8G8R8A8Srgb;
  vk::Format depth_format = vk::Format::eD32Sfloat;
  vk::ColorSpaceKHR color_space = vk::ColorSpaceKHR::eSrgbNonlinear;
  vk::PresentModeKHR present_mode = vk::PresentModeKHR::eMailbox;
  vk::Extent2D extent = vk::Extent2D{ 1280, 720 };
  std::array<float, 4> background_color = { 0.0, 0.0, 0.0, 1.0 };
};

class Engine {
  friend class MaterialManager;
  friend class VulkanContext;

  public:
    Engine();
    Engine(const Settings&);
    Engine(Settings&&);
    Engine(Engine&) = delete;
    Engine(Engine&&) = delete;

    ~Engine();

    Engine& operator = (Engine&) = delete;
    Engine& operator = (Engine&&) = delete;

    void add_material(std::string, const MaterialManager::Builder&);
    void add_material(std::string, MaterialManager::Builder&&);
    void run();

    template <typename Func>
    void run(Func&& code) {
      load();

      while (!shouldClose()) {
        pollEvents();
        code();
      }

      m_context.device().waitIdle();
    }

  private:
    bool shouldClose() const;
    void pollEvents() const;

    void initialize();
    void createWindow();
    void createSurface();
    void load();

  private:
    Settings m_settings;

    VulkanContext m_context;
    MaterialManager m_materials;

    GLFWwindow * m_window = nullptr;
    vk::raii::SurfaceKHR m_surface = nullptr;
};

} // namespace ge