#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <map>
#include <mutex>
#include <string>

#define ge_materials ge::Materials::instance()
#define ge_material_builder ge::Materials::builder()

namespace ge {

enum ShaderType {
  ComputeShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eCompute),
  VertexShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eVertex),
  FragmentShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eFragment)
};

class Materials {
  friend class Context;
  friend class Objects;
  friend class Renderer;

  using ShaderStages = std::pair<
    std::vector<vk::raii::ShaderModule>,
    std::vector<vk::PipelineShaderStageCreateInfo>
  >;

  private:
    class MaterialBuilder {
      friend class Materials;

      public:
        MaterialBuilder() = default;
        MaterialBuilder(const MaterialBuilder&) = default;
        MaterialBuilder(MaterialBuilder&&) = default;

        ~MaterialBuilder() = default;

        MaterialBuilder& operator = (const MaterialBuilder&) = default;
        MaterialBuilder& operator = (MaterialBuilder&&) = default;

        MaterialBuilder& add_shader(ShaderType, std::string);

        #ifdef ge_tests

          const std::map<vk::ShaderStageFlagBits, std::string>& get_shaders() const { return shaders; }

        #endif

      private:
        std::map<vk::ShaderStageFlagBits, std::string> shaders;
    };

    struct MaterialData {
      unsigned int pipelineIndex;
    };

  public:
    Materials(Materials&) = delete;
    Materials(Materials&&) = delete;

    Materials& operator = (Materials&) = delete;
    Materials& operator = (Materials&&) = delete;

    static Materials& instance();
    static void destroy();
    static MaterialBuilder builder();

    void add(std::string, MaterialBuilder&);
    void add(std::string, MaterialBuilder&&);

    #ifdef ge_tests

      const std::map<std::string, MaterialBuilder>& get_builders() const { return builders; }
      const std::map<std::string, MaterialData>& get_materials() const { return materials; }
      const vk::raii::PipelineLayout& get_layout() const { return layout; }
      const std::vector<vk::raii::Pipeline>& get_pipelines() const { return pipelines; }

    #endif // ge_tests

  private:
    Materials() = default;
    ~Materials() = default;

    ShaderStages getShaderStages(const std::map<vk::ShaderStageFlagBits, std::string>& shaders) const;

    void load();
    void createLayout();
    void createPipeline(const std::map<vk::ShaderStageFlagBits, std::string>& shaders);

  private:
    static std::mutex mutex;
    static Materials * p_materials;

    std::map<std::string, MaterialBuilder> builders;
    std::map<std::string, MaterialData> materials;

    vk::raii::PipelineLayout layout = nullptr;

    std::vector<vk::raii::Pipeline> pipelines;
};

};