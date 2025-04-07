#include "src/include/allocator.hpp"
#include "src/include/materials.hpp"
#include "src/include/objects.hpp"
#include "src/include/parsers.hpp"

#include <mutex>

namespace ge {

std::mutex Objects::mutex;
Objects * Objects::p_objects = nullptr;

Objects& Objects::instance() {
  if (p_objects == nullptr) {
    std::lock_guard lock(mutex);
    p_objects = new Objects();
  }

  return *p_objects;
}

void Objects::destroy() {
  if (p_objects == nullptr) return;

  std::lock_guard lock(mutex);

  delete p_objects;
  p_objects = nullptr;
}

void Objects::add(std::string material, std::string model) {
  if (!ge_materials.builders.contains(material))
    throw std::runtime_error("groot-engine: material '" + material + "' not found");

  auto [vertices, indices] = ObjParser::parse(model);

  ObjectData& obj = objectMap[material];
  obj.commands.emplace_back(IndirectCommand{
    .indexCount     = static_cast<unsigned int>(indices.size()),
    .instanceCount  = 1,
    .firstIndex     = static_cast<unsigned int>(obj.indices.size()),
    .vertexOffset   = static_cast<unsigned int>(obj.vertices.size()),
    .firstInstance  = 0
  });
  obj.vertices.insert(obj.vertices.end(), vertices.begin(), vertices.end());
  obj.indices.insert(obj.indices.end(), indices.begin(), indices.end());
}

void Objects::load() {
  if (objectMap.empty()) return;

  std::vector<vk::BufferCreateInfo> vertexInfos;
  std::vector<vk::BufferCreateInfo> indexInfos;
  std::vector<vk::BufferCreateInfo> indirectInfos;

  for (auto& [_, obj] : objectMap) {
    obj.bufIndices = { vertexInfos.size(), indexInfos.size(), indirectInfos.size() };

    vertexInfos.emplace_back(vk::BufferCreateInfo{
      .size         = obj.vertices.size() * sizeof(Vertex),
      .usage        = vk::BufferUsageFlagBits::eTransferSrc,
      .sharingMode  = vk::SharingMode::eExclusive
    });

    indexInfos.emplace_back(vk::BufferCreateInfo{
      .size         = obj.indices.size() * sizeof(unsigned int),
      .usage        = vk::BufferUsageFlagBits::eTransferSrc,
      .sharingMode  = vk::SharingMode::eExclusive
    });

    indirectInfos.emplace_back(vk::BufferCreateInfo{
      .size         = obj.commands.size() * sizeof(IndirectCommand),
      .usage        = vk::BufferUsageFlagBits::eTransferSrc,
      .sharingMode  = vk::SharingMode::eExclusive
    });
  }

  auto [vtMem, vtBufs, vtOffs, vtSize] = Allocator::bufferPool(vertexInfos,
    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
  );

  auto [itMem, itBufs, itOffs, itSize] = Allocator::bufferPool(indexInfos,
    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
  );

  auto [intMem, intBufs, intOffs, intSize] = Allocator::bufferPool(indirectInfos,
    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
  );

  void * vtMap = vtMem.mapMemory(0, vtSize);
  void * itMap = itMem.mapMemory(0, itSize);
  void * intMap = intMem.mapMemory(0, intSize);

  std::vector<std::tuple<vk::BufferCopy, vk::BufferCopy, vk::BufferCopy>> copies;
  for (const auto& [_, obj] : objectMap) {
    const auto& [vIndex, iIndex, inIndex] = obj.bufIndices;

    memcpy(reinterpret_cast<char *>(vtMap) + vtOffs[vIndex], obj.vertices.data(), obj.vertices.size() * sizeof(Vertex));
    memcpy(reinterpret_cast<char *>(itMap) + itOffs[iIndex], obj.indices.data(), obj.indices.size() * sizeof(unsigned int));
    memcpy(reinterpret_cast<char *>(intMap) + intOffs[inIndex], obj.commands.data(), obj.commands.size() * sizeof(IndirectCommand));

    vertexInfos[vIndex].usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
    indexInfos[iIndex].usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst;
    indirectInfos[inIndex].usage = vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eTransferDst;

    copies.emplace_back(std::tuple<vk::BufferCopy, vk::BufferCopy, vk::BufferCopy>{
      vk::BufferCopy{ .size = obj.vertices.size() * sizeof(Vertex) },
      vk::BufferCopy{ .size = obj.indices.size() * sizeof(unsigned int) },
      vk::BufferCopy{ .size = obj.commands.size() * sizeof(IndirectCommand) }
    });
  }

  vtMem.unmapMemory();
  itMem.unmapMemory();
  intMem.unmapMemory();
  vtMap = itMap = intMap = nullptr;

  auto [tmp_vMem, tmp_vBufs, _1, _2] = Allocator::bufferPool(vertexInfos, vk::MemoryPropertyFlagBits::eDeviceLocal);
  vertexMemory = std::move(tmp_vMem);
  vertexBuffers = std::move(tmp_vBufs);

  auto [tmp_iMem, tmp_iBufs, _3, _4] = Allocator::bufferPool(indexInfos, vk::MemoryPropertyFlagBits::eDeviceLocal);
  indexMemory = std::move(tmp_iMem);
  indexBuffers = std::move(tmp_iBufs);

  auto [tmp_inMem, tmp_inBufs, _5, _6] = Allocator::bufferPool(indirectInfos, vk::MemoryPropertyFlagBits::eDeviceLocal);
  indirectMemory = std::move(tmp_inMem);
  indirectBuffers = std::move(tmp_inBufs);

  vk::raii::CommandBuffer transferCmd = std::move(Allocator::transferBuffers(1)[0]);
  transferCmd.begin({ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });

  unsigned int index = 0;
  for (const auto& [vCopy, iCopy, inCopy] : copies) {
    transferCmd.copyBuffer(vtBufs[index], vertexBuffers[index], vCopy);
    transferCmd.copyBuffer(itBufs[index], indexBuffers[index], iCopy);
    transferCmd.copyBuffer(intBufs[index], indirectBuffers[index], inCopy);
    ++index;
  }

  transferCmd.end();

  vk::SubmitInfo transferSubmit{
    .commandBufferCount = 1,
    .pCommandBuffers    = &*transferCmd
  };

  vk::raii::Fence transferFence = Context::device().createFence({});

  Context::queue(Transfer).submit(transferSubmit, transferFence);

  if (Context::device().waitForFences(*transferFence, true, ge_timeout) != vk::Result::eSuccess)
    throw std::runtime_error("groot-engine: hung waiting for ge_objects buffer transfer");
}

} // namespace ge