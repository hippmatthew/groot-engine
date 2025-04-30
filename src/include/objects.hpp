#pragma once

#include "src/include/transform.hpp"
#include "src/include/vertex.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <map>
#include <vector>

namespace ge {

using transform = Transform&;

class Engine;

class ObjectManager {
  using Output = std::tuple<
    const vk::raii::Buffer&,
    const vk::raii::Buffer&,
    const vk::raii::Buffer&,
    const unsigned int,
    const unsigned int&
  >;

  private:
    struct IndirectCommand {
      unsigned int indexCount = 0;
      unsigned int instanceCount = 0;
      unsigned int firstIndex = 0;
      unsigned int vertexOffset = 0;
      unsigned int firstInstance = 0;
    };

    struct ObjectData {
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      unsigned int bufferIndex = 0;
      std::vector<IndirectCommand> commands;
      std::vector<Transform> transforms;
      unsigned int transformIndex = 0;
    };

  public:
    ObjectManager() = default;
    ObjectManager(ObjectManager&) = delete;
    ObjectManager(ObjectManager&&) = delete;

    ~ObjectManager() = default;

    ObjectManager& operator=(ObjectManager&) = delete;
    ObjectManager& operator=(ObjectManager&&) = delete;

    const Output operator[](std::string) const;

    bool hasObjects(std::string) const;
    unsigned int commandSize() const;

    Transform& add(const std::string&, const std::string&, const Transform&);
    std::vector<mat4> transforms();
    void load(const Engine&);

  private:
    std::map<std::string, ObjectData> m_objects;

    vk::raii::DeviceMemory m_vertexMemory = nullptr;
    std::vector<vk::raii::Buffer> m_vertexBuffers;

    vk::raii::DeviceMemory m_indexMemory = nullptr;
    std::vector<vk::raii::Buffer> m_indexBuffers;

    vk::raii::DeviceMemory m_indirectMemory = nullptr;
    std::vector<vk::raii::Buffer> m_indirectBuffers;
};

} // namespace ge