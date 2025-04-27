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

Allocator::DepthOutput Allocator::depthResources(const Engine & engine) {
  vk::FormatProperties properties = engine.m_context.gpu().getFormatProperties(engine.m_settings.depth_format);
  if (!(properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment))
    throw std::runtime_error("groot-engine: invalid depth format");

  vk::raii::DeviceMemory memory = nullptr;
  vk::raii::Image image = nullptr;
  vk::raii::ImageView view = nullptr;

  image = engine.m_context.device().createImage(vk::ImageCreateInfo{
    .imageType  = vk::ImageType::e2D,
    .format     = engine.m_settings.depth_format,
    .extent     = {
      .width  = engine.m_settings.extent.width,
      .height = engine.m_settings.extent.height,
      .depth  = 1
    },
    .mipLevels    = 1,
    .arrayLayers  = 1,
    .samples      = vk::SampleCountFlagBits::e1,
    .tiling       = vk::ImageTiling::eOptimal,
    .usage        = vk::ImageUsageFlagBits::eDepthStencilAttachment,
    .sharingMode  = vk::SharingMode::eExclusive
  });

  vk::MemoryRequirements requirements = image.getMemoryRequirements();
  unsigned int allocationSize = requirements.size;
  unsigned int filter = requirements.memoryTypeBits;

  memory = allocate(engine, allocationSize, filter, vk::MemoryPropertyFlagBits::eDeviceLocal);
  image.bindMemory(memory, 0);

  view = engine.m_context.device().createImageView(vk::ImageViewCreateInfo{
    .image            = image,
    .viewType         = vk::ImageViewType::e2D,
    .format           = engine.m_settings.depth_format,
    .subresourceRange = {
      .aspectMask = vk::ImageAspectFlagBits::eDepth,
      .levelCount = 1,
      .layerCount = 1
    }
  });

  return { std::move(memory), std::move(image), std::move(view) };
}

Allocator::FenceOutput Allocator::fences(const Engine& engine, unsigned int count, bool signaled) {
  std::vector<vk::raii::Fence> fences;

  for (unsigned int i = 0; i < count; ++i) {
    fences.emplace_back(engine.m_context.device().createFence(vk::FenceCreateInfo{
      .flags = signaled ? vk::FenceCreateFlagBits::eSignaled : vk::FenceCreateFlags()
    }));
  }

  return std::move(fences);
}

Allocator::SemaphoreOutput Allocator::semaphores(const Engine& engine, unsigned int count) {
  std::vector<vk::raii::Semaphore> semaphores;

  for (unsigned int i = 0; i < count; ++i)
    semaphores.emplace_back(engine.m_context.device().createSemaphore({}));

  return std::move(semaphores);
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