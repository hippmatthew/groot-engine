#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <map>
#include <string>

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

  public:
    MaterialManager() = default;
    MaterialManager(MaterialManager&) = delete;
    MaterialManager(MaterialManager&&) = delete;

    ~MaterialManager() = default;

    MaterialManager& operator=(MaterialManager&) = delete;
    MaterialManager& operator=(MaterialManager&&) = delete;

    bool exists(std::string) const;

    void add(const std::string&, const Builder&);
    void add(const std::string&, Builder&&);
    void load(const Engine&);

  private:
    ShaderStages getShaderStages(const Engine&, const Builder&) const;

    void createLayout(const Engine&);
    void createPipeline(const Engine&, const Builder&);

  private:
    std::map<std::string, Material> m_materials;
    std::vector<Builder> m_builders;

    vk::raii::PipelineLayout m_layout = nullptr;
    std::vector<vk::raii::Pipeline> m_pipelines;
};

} // namespace ge