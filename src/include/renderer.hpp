#pragma once

#include "src/include/materials.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <vector>

namespace ge {

class Engine;

class Renderer {
  public:
    Renderer() = default;
    Renderer(Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    ~Renderer() = default;

    Renderer& operator = (Renderer&) = delete;
    Renderer& operator = (Renderer&&) = delete;

    void initialize(Engine&);
    void render(const Engine&);

  private:
    void checkFormat(Engine&) const;
    void checkPresentMode(Engine&) const;
    std::pair<unsigned int, vk::SurfaceTransformFlagBitsKHR> checkExtent(Engine&) const;

    void createSwapchain(const Engine&, const unsigned int&, const vk::SurfaceTransformFlagBitsKHR&);
    void createViews(const Engine&);
    void createSyncObjects(const Engine&);
    void transitionImages(const unsigned int&);
    void preDraw(const Engine&, const unsigned int&);
    void draw(const Engine&);
    void endRendering();

  private:
    unsigned int m_frameIndex = 0;

    vk::raii::SwapchainKHR m_swapchain = nullptr;
    std::vector<vk::Image> m_images;
    std::vector<vk::raii::ImageView> m_views;

    vk::raii::DeviceMemory m_depthMem = nullptr;
    vk::raii::Image m_depthImage = nullptr;
    vk::raii::ImageView m_depthView = nullptr;

    vk::raii::CommandBuffers m_renderCmds = nullptr;

    std::vector<vk::raii::Fence> m_flightFences;
    std::vector<vk::raii::Semaphore> m_renderSemaphores;
    std::vector<vk::raii::Semaphore> m_imageSemaphores;

    PushConstants m_constants;
};

} // namespace ge