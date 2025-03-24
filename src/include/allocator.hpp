#pragma once

#include "src/include/context.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

namespace ge {

class Allocator {
  using BufferPool = std::tuple<
    vk::raii::DeviceMemory,
    std::vector<vk::raii::Buffer>,
    std::vector<unsigned int>,
    unsigned int
  >;

  using ImagePool = std::tuple<
    vk::raii::DeviceMemory,
    std::vector<vk::raii::Image>,
    std::vector<vk::raii::ImageView>
  >;

  using CommandPool = std::pair<
    vk::raii::CommandPool,
    vk::raii::CommandBuffers
  >;

  using DescriptorPool = std::pair<
    vk::raii::DescriptorPool,
    std::vector<vk::raii::DescriptorSets>
  >;

  public:
    Allocator() = delete;
    Allocator(Allocator&) = delete;
    Allocator(Allocator&&) = delete;

    ~Allocator()  = default;

    Allocator& operator = (Allocator&) = delete;
    Allocator& operator = (Allocator&&) = delete;

    static BufferPool bufferPool(const std::vector<vk::BufferCreateInfo>&, vk::MemoryPropertyFlags);
    static ImagePool imagePool(const std::vector<vk::ImageCreateInfo>&);
    static CommandPool commandPool(QueueFamilyType, vk::CommandPoolCreateFlags, unsigned int);
    static vk::raii::CommandBuffers transferBuffers(unsigned int);
    static DescriptorPool descriptorPool(
      const std::vector<vk::raii::DescriptorSetLayout>&,
      std::tuple<unsigned int, unsigned int, unsigned int, unsigned int>
    );

  private:
    static unsigned int findIndex(unsigned int&, vk::MemoryPropertyFlags&);
    static vk::raii::DeviceMemory allocate(unsigned int&, unsigned int&, vk::MemoryPropertyFlags);
    static vk::raii::CommandPool createCommandPool(unsigned int, vk::CommandPoolCreateFlags);
    static vk::raii::CommandBuffers createCommandBuffers(vk::raii::CommandPool&, unsigned int);

  private:
    static vk::raii::CommandPool transferPool;
};

} // namespace hlvl