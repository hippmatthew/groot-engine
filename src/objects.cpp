#include "src/include/allocator.hpp"
#include "src/include/engine.hpp"
#include "src/include/objects.hpp"
#include "src/include/parsers.hpp"

namespace ge {

void ObjectManager::add(const std::string& material, const std::string& path) {
  auto [vertices, indices] = ObjParser::parse(path);

  ObjectData& obj = m_objects[material];
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

void ObjectManager::load(const Engine& engine) {
  if (m_objects.empty()) return;

  std::vector<vk::BufferCreateInfo> vertInfos;
  std::vector<vk::BufferCreateInfo> indInfos;
  std::vector<vk::BufferCreateInfo> indirInfos;

  for (auto& [material, obj] : m_objects) {
    obj.bufferIndex = vertInfos.size();

    vertInfos.emplace_back(vk::BufferCreateInfo{
      .size         = obj.vertices.size() * sizeof(Vertex),
      .usage        = vk::BufferUsageFlagBits::eTransferSrc,
      .sharingMode  = vk::SharingMode::eExclusive
    });

    indInfos.emplace_back(vk::BufferCreateInfo{
      .size         = obj.indices.size() * sizeof(unsigned int),
      .usage        = vk::BufferUsageFlagBits::eTransferSrc,
      .sharingMode  = vk::SharingMode::eExclusive
    });

    indirInfos.emplace_back(vk::BufferCreateInfo{
      .size         = obj.commands.size() * sizeof(IndirectCommand),
      .usage        = vk::BufferUsageFlagBits::eTransferSrc,
      .sharingMode  = vk::SharingMode::eExclusive
    });
  }

  auto [vertTransMem, vertTransBufs, vertTransOffs, vertTransSize] = Allocator::bufferPool(engine, vertInfos,
    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
  );

  auto [indTransMem, indTransBufs, indTransOffs, indTransSize] = Allocator::bufferPool(engine, indInfos,
    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
  );

  auto [indirTransMem, indirTransBufs, indirTransOffs, indirTransSize] = Allocator::bufferPool(engine, indirInfos,
    vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
  );

  void * vertMap = vertTransMem.mapMemory(0, vertTransSize);
  void * indMap = indTransMem.mapMemory(0, indTransSize);
  void * indirMap = indirTransMem.mapMemory(0, indirTransSize);

  std::vector<std::tuple<vk::BufferCopy, vk::BufferCopy, vk::BufferCopy>> copies;
  for (const auto& [material, obj] : m_objects) {
    memcpy(reinterpret_cast<char *>(vertMap) + vertTransOffs[obj.bufferIndex], obj.vertices.data(), obj.vertices.size() * sizeof(Vertex));
    memcpy(reinterpret_cast<char *>(indMap) + indTransOffs[obj.bufferIndex], obj.indices.data(), obj.indices.size() * sizeof(unsigned int));
    memcpy(reinterpret_cast<char *>(indirMap) + indirTransOffs[obj.bufferIndex], obj.commands.data(), obj.commands.size() * sizeof(IndirectCommand));

    vertInfos[obj.bufferIndex].usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst;
    indInfos[obj.bufferIndex].usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst;
    indirInfos[obj.bufferIndex].usage = vk::BufferUsageFlagBits::eIndirectBuffer | vk::BufferUsageFlagBits::eTransferDst;

    copies.emplace_back(std::tuple<vk::BufferCopy, vk::BufferCopy, vk::BufferCopy>{
      vk::BufferCopy{ .size = obj.vertices.size() * sizeof(Vertex) },
      vk::BufferCopy{ .size = obj.indices.size() * sizeof(unsigned int) },
      vk::BufferCopy{ .size = obj.commands.size() * sizeof(IndirectCommand) }
    });
  }

  vertTransMem.unmapMemory();
  indTransMem.unmapMemory();
  indirTransMem.unmapMemory();
  vertMap = indMap = indirMap = nullptr;

  auto [tmp_vertMem, tmp_vertBufs, _v1, _v2] = Allocator::bufferPool(engine, vertInfos,
    vk::MemoryPropertyFlagBits::eDeviceLocal
  );
  m_vertexMemory = std::move(tmp_vertMem);
  m_vertexBuffers = std::move(tmp_vertBufs);

  auto [tmp_indMem, tmp_indBufs, _i1, _i2] = Allocator::bufferPool(engine, indInfos,
    vk::MemoryPropertyFlagBits::eDeviceLocal
  );
  m_indexMemory = std::move(tmp_indMem);
  m_indexBuffers = std::move(tmp_indBufs);

  auto [tmp_indirMem, tmp_indirBufs, _in1, _in2] = Allocator::bufferPool(engine, indirInfos,
    vk::MemoryPropertyFlagBits::eDeviceLocal
  );
  m_indirectMemory = std::move(tmp_indirMem);
  m_indirectBuffers = std::move(tmp_indirBufs);

  vk::raii::CommandBuffer transferCmd = std::move(engine.getCmds(QueueFamilyType::Transfer, 1)[0]);
  transferCmd.begin({ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });

  unsigned int index = 0;
  for (const auto& [vertCopy, indCopy, indirCopy] : copies) {
    transferCmd.copyBuffer(vertTransBufs[index], m_vertexBuffers[index], vertCopy);
    transferCmd.copyBuffer(indTransBufs[index], m_indexBuffers[index], indCopy);
    transferCmd.copyBuffer(indirTransBufs[index], m_indirectBuffers[index], indirCopy);
    ++index;
  }

  transferCmd.end();

  vk::SubmitInfo transferSubmit{
    .commandBufferCount = 1,
    .pCommandBuffers    = &*transferCmd
  };

  vk::raii::Fence transferFence = engine.m_context.device().createFence({});

  engine.m_context.queueFamily(QueueFamilyType::Transfer).queue.submit(transferSubmit, transferFence);

  if (engine.m_context.device().waitForFences(*transferFence, true, ge_timeout) != vk::Result::eSuccess)
    throw std::runtime_error("groot-engine: hung waiting for objects buffer transfer");
}

} // namespace ge