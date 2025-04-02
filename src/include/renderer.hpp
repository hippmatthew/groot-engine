#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <vector>

namespace ge {

class Renderer {
  friend class Context;

  public:
    Renderer() = default;
    Renderer(Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    ~Renderer() = default;

    Renderer& operator = (Renderer&) = delete;
    Renderer& operator = (Renderer&&) = delete;

  private:
    void checkFormat() const;
    void checkPresentMode() const;
    std::pair<unsigned int, vk::SurfaceTransformFlagBitsKHR> checkExtent() const;

    void destroy();
    void init();
    void createSwapchain(const unsigned int&, const vk::SurfaceTransformFlagBitsKHR&);
    void createViews();
    void createSyncObjects();
    void render();
    void transitionImages(const unsigned int&);
    void preDraw(const unsigned int&);
    void draw();
    void endRendering();


  private:
    unsigned int frameIndex = 0;

    vk::raii::SwapchainKHR swapchain = nullptr;
    std::vector<vk::Image> images;
    std::vector<vk::raii::ImageView> views;

    vk::raii::DeviceMemory depthMem = nullptr;
    vk::raii::Image depthImage = nullptr;
    vk::raii::ImageView depthView = nullptr;

    vk::raii::CommandPool renderPool = nullptr;
    vk::raii::CommandBuffers renderCmds = nullptr;

    std::vector<vk::raii::Fence> flightFences;
    std::vector<vk::raii::Semaphore> renderSemaphores;
    std::vector<vk::raii::Semaphore> imageSemaphores;
};

} // namespace ge