#pragma once

#include "src/include/vertex.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <map>
#include <mutex>
#include <vector>

#define ge_objects ge::Objects::instance()

namespace ge {

class Objects {
  friend class Context;
  friend class Materials;
  friend class Renderer;

  private:
    struct IndirectCommand {
      unsigned int indexCount;
      unsigned int instanceCount;
      unsigned int firstIndex;
      unsigned int vertexOffset;
      unsigned int firstInstance;
    };

    struct ObjectData {
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;
      std::tuple<unsigned int, unsigned int, unsigned int> bufIndices;
      std::vector<IndirectCommand> commands;
    };

  public:
    Objects(Objects&) = delete;
    Objects(Objects&&) = delete;

    Objects& operator = (Objects&) = delete;
    Objects& operator = (Objects&&) = delete;

    static Objects& instance();
    static void destroy();

    void add(std::string, std::string);

    #ifdef ge_tests

      const std::map<std::string, ObjectData>& get_objects() const { return objectMap; }
      const vk::raii::DeviceMemory& get_vMem() const { return vertexMemory; }
      const vk::raii::DeviceMemory& get_iMem() const { return indexMemory; }
      const vk::raii::DeviceMemory& get_inMem() const { return indirectMemory; }
      const std::vector<vk::raii::Buffer>& get_vBufs() const { return vertexBuffers; }
      const std::vector<vk::raii::Buffer>& get_iBufs() const { return indexBuffers; }
      const std::vector<vk::raii::Buffer>& get_inBufs() const { return indirectBuffers; }

    #endif // ge_tests

  private:
    Objects() = default;
    ~Objects() = default;

    void load();

  private:
    static std::mutex mutex;
    static Objects * p_objects;

    std::map<std::string, ObjectData> objectMap;

    vk::raii::DeviceMemory vertexMemory = nullptr;
    std::vector<vk::raii::Buffer> vertexBuffers;

    vk::raii::DeviceMemory indexMemory = nullptr;
    std::vector<vk::raii::Buffer> indexBuffers;

    vk::raii::DeviceMemory indirectMemory = nullptr;
    std::vector<vk::raii::Buffer> indirectBuffers;
};

} // namespace ge