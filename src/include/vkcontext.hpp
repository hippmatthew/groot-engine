#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <map>

namespace ge {

class Engine;

enum QueueFamilyType {
  Main = static_cast<unsigned int>(vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer),
  Async = static_cast<unsigned int>(vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer),
  Compute = static_cast<unsigned int>(vk::QueueFlagBits::eCompute),
  Transfer = static_cast<unsigned int>(vk::QueueFlagBits::eTransfer),
};

struct QueueFamily {
  unsigned int index = -1;
  vk::raii::Queue queue = nullptr;
};

class VulkanContext {
  using QueueFamilies = std::map<QueueFamilyType, QueueFamily>;

  public:
    VulkanContext() = default;
    VulkanContext(VulkanContext&) = delete;
    VulkanContext(VulkanContext&&) = delete;

    ~VulkanContext() = default;

    VulkanContext& operator=(VulkanContext&) = delete;
    VulkanContext& operator=(VulkanContext&&) = delete;

    const vk::raii::Instance& instance() const;
    const vk::raii::PhysicalDevice& gpu() const;
    const vk::raii::Device& device() const;
    const QueueFamily& queueFamily(QueueFamilyType) const;

    void initialize(const Engine&);

  private:
    void createInstance(const Engine&);
    void chooseGPU(const Engine&, const std::vector<const char *>&);
    void getQueueFamilies(const Engine&);
    void createDevice(const Engine&, std::vector<const char *>&);

  private:
    vk::raii::Instance m_instance = nullptr;
    vk::raii::PhysicalDevice m_gpu = nullptr;
    vk::raii::Device m_device = nullptr;
    QueueFamilies m_queueFamilies;
};

} // namespace ge