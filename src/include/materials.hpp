#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

#include <map>
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

      private:
        std::string tag;
        std::map<vk::ShaderStageFlagBits, std::string> shaders;
    };

  public:
    Material() = delete;
    Material(Material&) = delete;
    Material(Material&&) = default;
    Material(const MaterialBuilder&);

    ~Material() = default;

    Material& operator = (Material&) = delete;
    Material& operator = (Material&&) = default;

  private:
    std::vector<char> readShader(const std::string&) const;
    ShaderStages getShaderStages(const std::map<vk::ShaderStageFlagBits, std::string>&) const;

    void createLayout(const MaterialBuilder&);
    void createGPipeline(const MaterialBuilder&);

  private:
    vk::raii::PipelineLayout layout = nullptr;
    vk::raii::Pipeline gPipeline = nullptr;
};

class Materials {
  friend class Context;

  public:
    Materials(Materials&) = delete;
    Materials(Materials&&) = delete;

    Materials& operator = (Materials&) = delete;
    Materials& operator = (Materials&&) = delete;

    static Materials& instance();
    static void destroy();

    void add(Material::MaterialBuilder&&);

  private:
    Materials() = default;
    ~Materials() = default;

    void load();

  private:
    static Materials * p_materials;

    std::vector<Material::MaterialBuilder> builders;
    std::map<std::string, Material> materialMap;
};

};