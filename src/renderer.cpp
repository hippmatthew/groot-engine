#include "src/include/allocator.hpp"
#include "src/include/context.hpp"
#include "src/include/materials.hpp"
#include "src/include/objects.hpp"
#include "src/include/renderer.hpp"
#include "src/include/settings.hpp"

namespace ge {

void Renderer::checkFormat() const {
  std::vector<vk::SurfaceFormatKHR> formats = Context::physicalDevice().getSurfaceFormatsKHR(Context::surface());

  for (const auto& format : formats) {
    if (format.format == ge_settings.format && format.colorSpace == ge_settings.color_space)
      return;
  }

  ge_settings.format = formats[0].format;
  ge_settings.color_space = formats[0].colorSpace;
}

void Renderer::checkPresentMode() const {
  for (const auto& presentMode : Context::physicalDevice().getSurfacePresentModesKHR(Context::surface())) {
    if (presentMode == ge_settings.present_mode)
      return;
  }

  ge_settings.present_mode = vk::PresentModeKHR::eFifo;
}

std::pair<unsigned int, vk::SurfaceTransformFlagBitsKHR> Renderer::checkExtent() const {
  vk::SurfaceCapabilitiesKHR surfaceCapabilities = Context::physicalDevice().getSurfaceCapabilitiesKHR(Context::surface());

  unsigned int imageCount = surfaceCapabilities.minImageCount + 1;
  if (imageCount > surfaceCapabilities.maxImageCount && surfaceCapabilities.maxImageCount > 0)
    imageCount = surfaceCapabilities.maxImageCount;

  if (surfaceCapabilities.currentExtent.width != std::numeric_limits<unsigned int>::max()) {
    ge_settings.extent = surfaceCapabilities.currentExtent;
    return std::make_pair(imageCount, surfaceCapabilities.currentTransform);
  }

  int width, height;
  glfwGetFramebufferSize(Context::window(), &width, &height);

  ge_settings.extent = vk::Extent2D{
    std::clamp(static_cast<unsigned int>(width),
      surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width
    ),
    std::clamp(static_cast<unsigned int>(height),
      surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height
    )
  };

  return std::make_pair(imageCount, surfaceCapabilities.currentTransform);
}

void Renderer::init() {
  checkFormat();
  checkPresentMode();
  auto [imageCount, transform] = checkExtent();

  createSwapchain(imageCount, transform);
  createViews();

  auto [tmp_dMem, tmp_dImg, tmp_dView] = Allocator::depthResource();
  depthMem = std::move(tmp_dMem);
  depthImage = std::move(tmp_dImg);
  depthView = std::move(tmp_dView);

  auto [tmp_rPool, tmp_rCmds] = Allocator::commandPool(
    Main, vk::CommandPoolCreateFlagBits::eResetCommandBuffer, ge_settings.buffer_mode
  );
  renderPool = std::move(tmp_rPool);
  renderCmds = std::move(tmp_rCmds);

  createSyncObjects();
}

void Renderer::createSwapchain(const unsigned int& imageCount, const vk::SurfaceTransformFlagBitsKHR& transform) {
  swapchain = Context::device().createSwapchainKHR(vk::SwapchainCreateInfoKHR{
    .surface            = Context::surface(),
    .minImageCount      = imageCount,
    .imageFormat        = ge_settings.format,
    .imageColorSpace    = ge_settings.color_space,
    .imageExtent        = ge_settings.extent,
    .imageArrayLayers   = 1,
    .imageUsage         = vk::ImageUsageFlagBits::eColorAttachment,
    .imageSharingMode   = vk::SharingMode::eExclusive,
    .preTransform       = transform,
    .compositeAlpha     = vk::CompositeAlphaFlagBitsKHR::eOpaque,
    .presentMode        = ge_settings.present_mode,
    .clipped            = true
  });

  images = std::move(swapchain.getImages());
}

void Renderer::createViews() {
  for (const auto& image : images) {
    views.emplace_back(Context::device().createImageView(vk::ImageViewCreateInfo{
      .image            = image,
      .viewType         = vk::ImageViewType::e2D,
      .format           = ge_settings.format,
      .subresourceRange = {
        .aspectMask = vk::ImageAspectFlagBits::eColor,
        .levelCount = 1,
        .layerCount = 1
      }
    }));
  }
}

void Renderer::createSyncObjects() {
  vk::FenceCreateInfo ci_flightFence{ .flags = vk::FenceCreateFlagBits::eSignaled };
  vk::SemaphoreCreateInfo ci_semaphore{};

  for (unsigned int i = 0; i < ge_settings.buffer_mode; ++i) {
    flightFences.emplace_back(Context::device().createFence(ci_flightFence));
    renderSemaphores.emplace_back(Context::device().createSemaphore(ci_semaphore));
    imageSemaphores.emplace_back(Context::device().createSemaphore(ci_semaphore));
  }
}

void Renderer::render() {
  if (Context::device().waitForFences(*flightFences[frameIndex], true, ge_timeout) != vk::Result::eSuccess)
    throw std::runtime_error("groot-engine: hung waiting for flight fence");

  auto [res, imgIndex] = swapchain.acquireNextImage(ge_timeout, imageSemaphores[frameIndex], nullptr);
  if (res != vk::Result::eSuccess) throw std::runtime_error("groot-engine: failed to get next swapchain image");

  Context::device().resetFences(*flightFences[frameIndex]);

  renderCmds[frameIndex].reset();
  renderCmds[frameIndex].begin({});

  transitionImages(imgIndex);
  preDraw(imgIndex);
  draw();

  renderCmds[frameIndex].end();

  const vk::PipelineStageFlags waitStage =  vk::PipelineStageFlagBits::eColorAttachmentOutput;
  vk::SubmitInfo renderSubmit{
    .waitSemaphoreCount   = 1,
    .pWaitSemaphores      = &*imageSemaphores[frameIndex],
    .pWaitDstStageMask    = &waitStage,
    .commandBufferCount   = 1,
    .pCommandBuffers      = &*renderCmds[frameIndex],
    .signalSemaphoreCount = 1,
    .pSignalSemaphores    = &*renderSemaphores[frameIndex]
  };

  Context::queue(Main).submit(renderSubmit, flightFences[frameIndex]);

  vk::PresentInfoKHR present{
    .waitSemaphoreCount = 1,
    .pWaitSemaphores    = &*renderSemaphores[frameIndex],
    .swapchainCount     = 1,
    .pSwapchains        = &*swapchain,
    .pImageIndices      = &imgIndex
  };

  if (Context::queue(Main).presentKHR(present) != vk::Result::eSuccess)
    throw std::runtime_error("groot-engine: failed to present image");

  frameIndex = (frameIndex + 1) % ge_settings.buffer_mode;
}

void Renderer::transitionImages(const unsigned int& imgIndex) {
  vk::ImageMemoryBarrier barrier{
    .dstAccessMask    = vk::AccessFlagBits::eColorAttachmentWrite,
    .newLayout        = vk::ImageLayout::eColorAttachmentOptimal,
    .image            = images[imgIndex],
    .subresourceRange = {
      .aspectMask = vk::ImageAspectFlagBits::eColor,
      .levelCount = 1,
      .layerCount = 1
    }
  };

  renderCmds[frameIndex].pipelineBarrier(
    vk::PipelineStageFlagBits::eTopOfPipe,
    vk::PipelineStageFlagBits::eColorAttachmentOutput,
    vk::DependencyFlags(),
    nullptr,
    nullptr,
    barrier
  );

  barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
  barrier.dstAccessMask = vk::AccessFlagBits::eNone;
  barrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal;
  barrier.newLayout = vk::ImageLayout::ePresentSrcKHR;

  renderCmds[frameIndex].pipelineBarrier(
    vk::PipelineStageFlagBits::eColorAttachmentOutput,
    vk::PipelineStageFlagBits::eBottomOfPipe,
    vk::DependencyFlags(),
    nullptr,
    nullptr,
    barrier
  );

  barrier.srcAccessMask = barrier.dstAccessMask;
  barrier.oldLayout = vk::ImageLayout::eUndefined;
  barrier.newLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
  barrier.image = depthImage;
  barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

  renderCmds[frameIndex].pipelineBarrier(
    vk::PipelineStageFlagBits::eTopOfPipe,
    vk::PipelineStageFlagBits::eEarlyFragmentTests,
    vk::DependencyFlags(),
    nullptr,
    nullptr,
    barrier
  );
}

void Renderer::preDraw(const unsigned int& imgIndex) {
  vk::RenderingAttachmentInfo color{
    .imageView    = views[imgIndex],
    .imageLayout  = vk::ImageLayout::eColorAttachmentOptimal,
    .loadOp       = vk::AttachmentLoadOp::eClear,
    .storeOp      = vk::AttachmentStoreOp::eStore,
    .clearValue   = { ge_settings.background_color }
  };

  vk::RenderingAttachmentInfo depth{
    .imageView    = depthView,
    .imageLayout  = vk::ImageLayout::eDepthAttachmentOptimal,
    .loadOp       = vk::AttachmentLoadOp::eClear,
    .storeOp      = vk::AttachmentStoreOp::eDontCare,
    .clearValue   = { .depthStencil = { 1, 0 } }
  };

  renderCmds[frameIndex].beginRendering(vk::RenderingInfo{
    .renderArea = {
      .extent = ge_settings.extent
    },
    .layerCount           = 1,
    .colorAttachmentCount = 1,
    .pColorAttachments    = &color,
    .pDepthAttachment     = &depth
  });

  renderCmds[frameIndex].setViewport(0, vk::Viewport{
    .x        = 0,
    .y        = 0,
    .width    = static_cast<float>(ge_settings.extent.width),
    .height   = static_cast<float>(ge_settings.extent.height),
    .minDepth = 0,
    .maxDepth = 1
  });

  renderCmds[frameIndex].setScissor(0, vk::Rect2D{ .extent = ge_settings.extent });
}

void Renderer::draw() {
  for (const auto& [tag, material] : ge_materials.materials) {
    renderCmds[frameIndex].bindPipeline(vk::PipelineBindPoint::eGraphics, ge_materials.pipelines[material.pipelineIndex]);

    const auto& obj = ge_objects.objectMap[tag];
    const auto& [vertexIndex, indexIndex, indirectIndex] = obj.bufIndices;

    renderCmds[frameIndex].bindVertexBuffers(0, *ge_objects.vertexBuffers[vertexIndex], { 0 });
    renderCmds[frameIndex].bindIndexBuffer(ge_objects.indexBuffers[indexIndex], 0, vk::IndexType::eUint32);

    renderCmds[frameIndex].drawIndexedIndirect(
      ge_objects.indirectBuffers[indirectIndex], 0, obj.commands.size(), sizeof(Objects::IndirectCommand)
    );
  }
  renderCmds[frameIndex].endRendering();
}

} // namespace ge