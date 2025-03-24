#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <string>

#define ge_settings ge::Settings::instance()

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

class Settings {
  public:
    Settings(Settings&) = delete;
    Settings(Settings&&) = delete;

    Settings& operator = (const Settings&);
    Settings& operator = (Settings&&) = delete;

    static Settings& instance();
    static void destroy();
    static void reset_to_default();

  private:
    Settings() = default;
    ~Settings() = default;

  public:
    unsigned int window_width = 1280;
    unsigned int window_height = 720;
    std::string window_title = "HLVL";

    std::string application_name = "HLVL Application";
    unsigned int application_version = ge_make_version(1, 0, 0);

    BufferMode buffer_mode = TripleBuffer;
    vk::Format format = vk::Format::eB8G8R8A8Srgb;
    vk::Format depth_format = vk::Format::eD32Sfloat;
    vk::ColorSpaceKHR color_space = vk::ColorSpaceKHR::eSrgbNonlinear;
    vk::PresentModeKHR present_mode = vk::PresentModeKHR::eMailbox;
    vk::Extent2D extent = vk::Extent2D{ 1280, 720 };
    std::array<float, 4> background_color = { 0.0, 0.0, 0.0, 1.0 };

  private:
    static Settings * p_settings;
};

} // namespace hlvl