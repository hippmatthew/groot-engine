#include "src/include/allocator.hpp"
#include "src/include/engine.hpp"

namespace ge {

Allocator::BufferOutput Allocator::bufferPool(
  const Engine& engine,
  const std::vector<vk::BufferCreateInfo>& infos,
  vk::MemoryPropertyFlags flags
) {
  vk::raii::DeviceMemory memory = nullptr;
  std::vector<vk::raii::Buffer> buffers;
  std::vector<unsigned int> offsets;
  unsigned int allocationSize = 0;
  unsigned int filter = ~(0x0);

  for (const auto& info : infos) {
    buffers.emplace_back(engine.m_context.device().createBuffer(info));
    vk::MemoryRequirements requirements = buffers.back().getMemoryRequirements();

    offsets.emplace_back((allocationSize + requirements.size - 1) / requirements.size * requirements.size);

    filter &= requirements.memoryTypeBits;
    allocationSize += offsets.back() + requirements.size;
  }

  memory = allocate(engine, allocationSize, filter, flags);

  for (unsigned int i = 0; i < buffers.size(); ++i)
    buffers[i].bindMemory(memory, offsets[i]);

  return { std::move(memory), std::move(buffers), std::move(offsets), std::move(allocationSize) };
}

vk::raii::DeviceMemory Allocator::allocate(
  const Engine& engine,
  const unsigned int& size,
  const unsigned int& filter,
  const vk::MemoryPropertyFlags& flags
) {
  vk::PhysicalDeviceMemoryProperties properties = engine.m_context.gpu().getMemoryProperties();

  unsigned int index = 0;
  for (const auto& memoryType : properties.memoryTypes) {
    if (index == properties.memoryTypeCount)
      throw std::runtime_error("groot-engine: unable to find suitable memory type index");

    if ((filter & (1 << index)) && ((memoryType.propertyFlags & flags) == flags))
      break;

    ++index;
  }

  return engine.m_context.device().allocateMemory(vk::MemoryAllocateInfo{
    .allocationSize   = size,
    .memoryTypeIndex  = index
  });
}

} // namespace ge