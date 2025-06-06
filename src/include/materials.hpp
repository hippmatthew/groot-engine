#pragma once

#include "src/include/linalg.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <map>
#include <string>

#define all_stages  vk::ShaderStageFlagBits::eVertex                  | \
                    vk::ShaderStageFlagBits::eGeometry                | \
                    vk::ShaderStageFlagBits::eTessellationControl     | \
                    vk::ShaderStageFlagBits::eTessellationEvaluation  | \
                    vk::ShaderStageFlagBits::eFragment                | \
                    vk::ShaderStageFlagBits::eCompute

namespace ge {

class Engine;

enum ShaderStage {
  VertexShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eVertex),
  GeometryShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eGeometry),
  TesselationControlShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eTessellationControl),
  TesselationEvaluationShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eTessellationEvaluation),
  FragmentShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eFragment),
  ComputeShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eCompute)
};

struct EngineData {
  mat4 view = mat4::identity();
  mat4 projection = mat4::identity();
  unsigned int frameIndex;
  unsigned int materialIndex;
  unsigned int transformIndex;
};

class MaterialManager {
  using ShaderStages = std::pair<
    std::vector<vk::raii::ShaderModule>,
    std::vector<vk::PipelineShaderStageCreateInfo>
  >;

  public:
    class Builder {
      friend class MaterialManager;

      public:
        Builder() = default;
        Builder(const Builder&) = default;
        Builder(Builder&&) = default;

        ~Builder() = default;

        Builder& operator=(const Builder&) = default;
        Builder& operator=(Builder&&) = default;

        Builder& add_shader(ShaderStage, std::string);

      private:
        std::map<vk::ShaderStageFlagBits, std::string> m_shaders;
    };

  private:
    struct Material {
      unsigned int builder = 0;
      unsigned int pipeline = 0;
    };

    class Iterator {
      using Output = std::pair<const std::string&, const vk::raii::Pipeline&>;

      public:
        Iterator(const MaterialManager *, const std::map<std::string, Material>::const_iterator&);
        Iterator(const Iterator&) = default;
        Iterator(Iterator&&) = default;

        ~Iterator() = default;

        Iterator& operator=(const Iterator&) = default;
        Iterator& operator=(Iterator&&) = default;

        bool operator!=(const Iterator&) const;

        Output operator*() const;

        Iterator& operator++();

      private:
        const MaterialManager * m_manager;
        std::map<std::string, Material>::const_iterator m_iterator;
    };

  public:
    MaterialManager() = default;
    MaterialManager(MaterialManager&) = delete;
    MaterialManager(MaterialManager&&) = delete;

    ~MaterialManager() = default;

    MaterialManager& operator=(MaterialManager&) = delete;
    MaterialManager& operator=(MaterialManager&&) = delete;

    Iterator begin() const;
    Iterator end() const;

    bool exists(std::string) const;
    const vk::raii::PipelineLayout& layout() const;
    const vk::raii::DescriptorSet& descriptorSet(unsigned int) const;

    void add(const std::string&, const Builder&);
    void add(const std::string&, Builder&&);
    void load(const Engine&, const std::vector<mat4>&);
    void updateTransforms(const unsigned int&, const std::vector<mat4>&);

  private:
    ShaderStages getShaderStages(const Engine&, const Builder&) const;

    void createLayout(const Engine&, unsigned int);
    void createPipeline(const Engine&, const Builder&);
    void createDescriptors(const Engine&, const std::vector<mat4>&);
    void updateSets(const Engine&);

  private:
    std::map<std::string, Material> m_materials;
    std::vector<Builder> m_builders;

    vk::raii::DescriptorSetLayout m_setLayout = nullptr;
    vk::raii::PipelineLayout m_layout = nullptr;
    std::vector<vk::raii::Pipeline> m_pipelines;

    vk::raii::DescriptorPool m_setPool = nullptr;
    vk::raii::DescriptorSets m_sets = nullptr;

    vk::raii::DeviceMemory m_transformMemory = nullptr;
    std::vector<vk::raii::Buffer> m_transformBuffers;
    std::vector<unsigned int> m_transformOffsets;
    void * m_transformMap = nullptr;
};

} // namespace ge