#pragma once

#include "src/include/vkcontext.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <vector>
#include <map>

namespace ge {

class Allocator {
  using BufferOutput = std::tuple<
    vk::raii::DeviceMemory,
    std::vector<vk::raii::Buffer>,
    std::vector<unsigned int>,
    unsigned int
  >;

  using CommandOutput = std::map<QueueFamilyType, vk::raii::CommandPool>;

  public:
    Allocator() = delete;
    Allocator(Allocator&) = delete;
    Allocator(Allocator&&) = delete;

    ~Allocator() = default;

    Allocator& operator=(Allocator&) = delete;
    Allocator& operator=(Allocator&&) = delete;

    static BufferOutput bufferPool(const Engine&, const std::vector<vk::BufferCreateInfo>&, vk::MemoryPropertyFlags);
    static CommandOutput commandPools(const Engine&);

  private:
    static vk::raii::DeviceMemory allocate(
      const Engine&,
      const unsigned int&,
      const unsigned int&,
      const vk::MemoryPropertyFlags&
    );
};

} // namespace ge