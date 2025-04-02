#include "src/include/allocator.hpp"
#include "src/include/context.hpp"
#include "src/include/settings.hpp"
#include "vulkan/vulkan_enums.hpp"

namespace ge {

vk::raii::CommandPool Allocator::transferPool = nullptr;

void Allocator::destroy() {
  transferPool.clear();
}

Allocator::BufferPool Allocator::bufferPool(
  const std::vector<vk::BufferCreateInfo>& infos, vk::MemoryPropertyFlags flags
) {
  vk::raii::DeviceMemory memory = nullptr;
  std::vector<vk::raii::Buffer> buffers;
  std::vector<unsigned int> offsets;
  unsigned int allocationSize = 0;

  unsigned int filter = ~(0x0);

  for (const auto& info : infos) {
    buffers.emplace_back(Context::device().createBuffer(info));

    auto reqs = buffers[buffers.size() - 1].getMemoryRequirements();

    offsets.emplace_back((allocationSize + reqs.alignment - 1) / reqs.alignment * reqs.alignment);

    filter &= reqs.memoryTypeBits;
    allocationSize = offsets[offsets.size() - 1] + reqs.size;
  }

  memory = allocate(allocationSize, filter, flags);

  for (unsigned int i = 0; i < buffers.size(); ++i)
    buffers[i].bindMemory(memory, offsets[i]);

  return { std::move(memory), std::move(buffers), std::move(offsets), std::move(allocationSize) };
}

Allocator::ImagePool Allocator::imagePool(const std::vector<vk::ImageCreateInfo>& infos) {
  vk::raii::DeviceMemory memory = nullptr;
  std::vector<vk::raii::Image> images;
  std::vector<vk::raii::ImageView> views;

  std::vector<unsigned int> offsets;
  unsigned int allocationSize = 0;
  unsigned int filter = ~(0x0);

  for (const auto& info : infos) {
    images.emplace_back(Context::device().createImage(info));

    auto reqs = images[images.size() - 1].getMemoryRequirements();

    offsets.emplace_back((allocationSize + reqs.alignment - 1) / reqs.alignment * reqs.alignment);

    filter &= reqs.memoryTypeBits;
    allocationSize = offsets[offsets.size() - 1] + reqs.size;
  }

  memory = allocate(allocationSize, filter, vk::MemoryPropertyFlagBits::eDeviceLocal);

  unsigned int index = 0;
  for (const auto& image : images) {
    image.bindMemory(memory, offsets[index++]);

    views.emplace_back(Context::device().createImageView(vk::ImageViewCreateInfo{
      .image      = image,
      .viewType   = vk::ImageViewType::e2D,
      .format     = vk::Format::eR8G8B8A8Srgb,
      .subresourceRange = {
        .aspectMask     = vk::ImageAspectFlagBits::eColor,
        .levelCount     = 1,
        .layerCount     = 1
      }
    }));
  }

  return { std::move(memory), std::move(images), std::move(views) };
}

Allocator::DepthResource Allocator::depthResource() {
  vk::FormatProperties properties = Context::physicalDevice().getFormatProperties(ge_settings.depth_format);
  if (!(properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment))
    throw std::runtime_error("groot-engine: invalid depth format");

  vk::raii::DeviceMemory memory = nullptr;
  vk::raii::Image image = nullptr;
  vk::raii::ImageView view = nullptr;

  image = Context::device().createImage(vk::ImageCreateInfo{
    .imageType  = vk::ImageType::e2D,
    .format     = ge_settings.depth_format,
    .extent     = {
      .width  = ge_settings.extent.width,
      .height = ge_settings.extent.height,
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

  memory = allocate(allocationSize, filter, vk::MemoryPropertyFlagBits::eDeviceLocal);
  image.bindMemory(memory, 0);

  view = Context::device().createImageView(vk::ImageViewCreateInfo{
    .image            = image,
    .viewType         = vk::ImageViewType::e2D,
    .format           = ge_settings.depth_format,
    .subresourceRange = {
      .aspectMask = vk::ImageAspectFlagBits::eDepth,
      .levelCount = 1,
      .layerCount = 1
    }
  });

  return { std::move(memory), std::move(image), std::move(view) };
}

Allocator::CommandPool Allocator::commandPool(QueueFamilyType type, vk::CommandPoolCreateFlags flags, unsigned int count) {
  vk::raii::CommandPool pool = createCommandPool(Context::queueIndex(type), flags);
  vk::raii::CommandBuffers buffers = createCommandBuffers(pool, count);

  return { std::move(pool), std::move(buffers) };
}

vk::raii::CommandBuffers Allocator::transferBuffers(unsigned int count) {
  if (*transferPool == nullptr)
    transferPool = createCommandPool(Context::queueIndex(Transfer), vk::CommandPoolCreateFlagBits::eTransient);

  return createCommandBuffers(transferPool, count);
}

Allocator::DescriptorPool Allocator::descriptorPool(
  const std::vector<vk::raii::DescriptorSetLayout>& layouts,
  std::tuple<unsigned int, unsigned int, unsigned int, unsigned int> counts
) {
  vk::raii::DescriptorPool pool = nullptr;
  std::vector<vk::raii::DescriptorSets> sets;

  auto [sampledCount, imgCount, storageCount, uniformCount] = counts;
  std::vector<vk::DescriptorPoolSize> poolSizes;

  if (sampledCount != 0) {
    poolSizes.emplace_back(vk::DescriptorPoolSize{
      .type = vk::DescriptorType::eCombinedImageSampler,
      .descriptorCount  = ge_settings.buffer_mode * sampledCount
    });
  }

  if (imgCount != 0) {
    poolSizes.emplace_back(vk::DescriptorPoolSize{
      .type = vk::DescriptorType::eStorageImage,
      .descriptorCount = ge_settings.buffer_mode * imgCount
    });
  }

  if (storageCount != 0) {
    poolSizes.emplace_back(vk::DescriptorPoolSize{
      .type = vk::DescriptorType::eStorageBuffer,
      .descriptorCount = ge_settings.buffer_mode * storageCount
    });
  }

  if (uniformCount != 0) {
    poolSizes.emplace_back(vk::DescriptorPoolSize{
      .type = vk::DescriptorType::eUniformBuffer,
      .descriptorCount  = ge_settings.buffer_mode * uniformCount
    });
  }

  pool = Context::device().createDescriptorPool(vk::DescriptorPoolCreateInfo{
    .flags          = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
    .poolSizeCount  = static_cast<unsigned int>(poolSizes.size()),
    .pPoolSizes     = poolSizes.data()
  });

  std::vector<vk::DescriptorSetLayout> baseLayouts;
  for (const auto& layout : layouts)
    baseLayouts.emplace_back(layout);

  for (unsigned int i = 0; i < ge_settings.buffer_mode; ++i) {
    sets.emplace_back(vk::raii::DescriptorSets(Context::device(), vk::DescriptorSetAllocateInfo{
      .descriptorPool     = pool,
      .descriptorSetCount = static_cast<unsigned int>(layouts.size()),
      .pSetLayouts        = baseLayouts.data()
    }));
  }

  return { std::move(pool), std::move(sets) };
}

unsigned int Allocator::findIndex(unsigned int& filter, vk::MemoryPropertyFlags& flags) {
  vk::PhysicalDeviceMemoryProperties properties = Context::physicalDevice().getMemoryProperties();

  unsigned int index = 0;
  for (const auto& memoryType : properties.memoryTypes) {
    if (index == properties.memoryTypeCount) break;

    if ((filter & (1 << index)) && ((memoryType.propertyFlags & flags) == flags))
      return index;
    ++index;
  }

  throw std::runtime_error("hlvl: failed to find a suitable memory index");
}

vk::raii::DeviceMemory Allocator::allocate(unsigned int& size, unsigned int& filter, vk::MemoryPropertyFlags flags) {
  vk::MemoryAllocateInfo info{
    .allocationSize   = size,
    .memoryTypeIndex  = findIndex(filter, flags)
  };

  return Context::device().allocateMemory(info);
}

vk::raii::CommandPool Allocator::createCommandPool(unsigned int index, vk::CommandPoolCreateFlags flags) {
  return Context::device().createCommandPool(vk::CommandPoolCreateInfo{
    .flags            = flags,
    .queueFamilyIndex = index
  });
}

vk::raii::CommandBuffers Allocator::createCommandBuffers(vk::raii::CommandPool& pool, unsigned int count) {
  return vk::raii::CommandBuffers(Context::device(), vk::CommandBufferAllocateInfo{
    .commandPool        = pool,
    .commandBufferCount = count
  });
}

} // namespace ge