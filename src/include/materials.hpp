#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <map>
#include <mutex>
#include <string>

#define ge_materials ge::Materials::instance()

namespace ge {

enum ShaderType {
  ComputeShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eCompute),
  VertexShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eVertex),
  FragmentShader = static_cast<unsigned int>(vk::ShaderStageFlagBits::eFragment)
};

class Material {
  friend class Materials;
  friend class Renderer;

  using ShaderStages = std::pair<
    std::vector<vk::raii::ShaderModule>,
    std::vector<vk::PipelineShaderStageCreateInfo>
  >;

  private:
    class MaterialBuilder {
      friend class Material;
      friend class Materials;

      public:
        MaterialBuilder(std::string);
        MaterialBuilder(const MaterialBuilder&) = default;
        MaterialBuilder(MaterialBuilder&&) = default;

        ~MaterialBuilder() = default;

        MaterialBuilder& operator = (const MaterialBuilder&) = default;
        MaterialBuilder& operator = (MaterialBuilder&&) = default;

        MaterialBuilder& add_shader(ShaderType, std::string);

        #ifdef ge_tests

          const std::string& get_tag() const { return tag; }
          const std::map<vk::ShaderStageFlagBits, std::string>& get_shaders() const { return shaders; }

        #endif

      private:
        std::string tag;
        std::map<vk::ShaderStageFlagBits, std::string> shaders;
    };

  public:
    Material() = default;
    Material(Material&) = delete;
    Material(Material&&) = default;
    Material(const MaterialBuilder&);

    ~Material() = default;

    Material& operator = (Material&) = delete;
    Material& operator = (Material&&) = default;

    static MaterialBuilder builder(std::string);

    #ifdef ge_tests

      const vk::raii::PipelineLayout& get_layout() const { return layout; }
      const vk::raii::Pipeline& get_gPipeline() const { return gPipeline; }

    #endif // ge_tests

  private:
    ShaderStages getShaderStages(const std::map<vk::ShaderStageFlagBits, std::string>&) const;

    void createLayout(const MaterialBuilder&);
    void createGPipeline(const MaterialBuilder&);

  private:
    vk::raii::PipelineLayout layout = nullptr;
    vk::raii::Pipeline gPipeline = nullptr;
};

class Materials {
  friend class Context;
  friend class Objects;
  friend class Renderer;

  public:
    Materials(Materials&) = delete;
    Materials(Materials&&) = delete;

    Materials& operator = (Materials&) = delete;
    Materials& operator = (Materials&&) = delete;

    static Materials& instance();
    static void destroy();

    void add(Material::MaterialBuilder);

    #ifdef ge_tests

      const std::vector<Material::MaterialBuilder>& get_builders() const { return builders; }
      const std::map<std::string, Material>& get_map() const { return materialMap; }

    #endif // ge_tests

  private:
    Materials() = default;
    ~Materials() = default;

    void load();

  private:
    static std::mutex mutex;
    static Materials * p_materials;

    std::vector<Material::MaterialBuilder> builders;
    std::map<std::string, Material> materialMap;
};

};