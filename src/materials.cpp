#include "src/include/context.hpp"
#include "src/include/materials.hpp"
#include "src/include/parsers.hpp"
#include "src/include/settings.hpp"
#include "src/include/vertex.hpp"

namespace ge {

std::mutex Materials::mutex;
Materials * Materials::p_materials = nullptr;

Material::MaterialBuilder::MaterialBuilder(std::string t) : tag(t) {}

Material::MaterialBuilder& Material::MaterialBuilder::add_shader(ShaderType type, std::string path) {
  shaders[static_cast<vk::ShaderStageFlagBits>(type)] = path;
  return *this;
}

Material::Material(const MaterialBuilder& builder) {
  createLayout(builder);
  createGPipeline(builder);
}

Material::MaterialBuilder Material::builder(std::string tag) {
  return MaterialBuilder(tag);
}

Material::ShaderStages Material::getShaderStages(const std::map<vk::ShaderStageFlagBits, std::string>& shaders) const {
  std::vector<vk::raii::ShaderModule> modules;
  std::vector<vk::PipelineShaderStageCreateInfo> infos;

  for (auto [stage, path] : shaders) {
    std::vector<char> code = SPVParser::parse(path);

    modules.emplace_back(Context::device().createShaderModule(vk::ShaderModuleCreateInfo{
      .codeSize = static_cast<unsigned int>(code.size()),
      .pCode    = reinterpret_cast<unsigned int *>(code.data())
    }));

    infos.emplace_back(vk::PipelineShaderStageCreateInfo{
      .stage  = stage,
      .module = modules[modules.size() - 1],
      .pName  = "main"
    });
  }

  return { std::move(modules), std::move(infos) };
}

void Material::createLayout(const MaterialBuilder& builder) {
  layout = Context::device().createPipelineLayout(vk::PipelineLayoutCreateInfo{
    .setLayoutCount         = 0,
    .pSetLayouts            = nullptr,
    .pushConstantRangeCount = 0,
    .pPushConstantRanges    = nullptr
  });
}

void Material::createGPipeline(const MaterialBuilder& builder) {
  auto [modules, ci_stages] = getShaderStages(builder.shaders);

  vk::DynamicState dynStates[2] = {
    vk::DynamicState::eViewport,
    vk::DynamicState::eScissor
  };

  vk::PipelineDynamicStateCreateInfo ci_dynState{
    .dynamicStateCount  = 2,
    .pDynamicStates     = dynStates
  };

  vk::PipelineViewportStateCreateInfo ci_viewport{
    .viewportCount  = 1,
    .scissorCount   = 1
  };

  auto vertBinding = Vertex::binding();
  auto vertAttribs = Vertex::attributes();

  vk::PipelineVertexInputStateCreateInfo ci_input{
    .vertexBindingDescriptionCount    = 1,
    .pVertexBindingDescriptions       = &vertBinding,
    .vertexAttributeDescriptionCount  = static_cast<unsigned int>(vertAttribs.size()),
    .pVertexAttributeDescriptions     = vertAttribs.data()
  };

  vk::PipelineInputAssemblyStateCreateInfo ci_assembly{
    .topology               = vk::PrimitiveTopology::eTriangleList,
    .primitiveRestartEnable = false
  };

  vk::PipelineRasterizationStateCreateInfo ci_rasterizer{
    .depthClampEnable         = false,
    .rasterizerDiscardEnable  = false,
    .polygonMode              = vk::PolygonMode::eFill,
    .cullMode                 = vk::CullModeFlagBits::eBack,
    .frontFace                = vk::FrontFace::eCounterClockwise,
    .depthBiasEnable          = false,
    .lineWidth                = 1.0f
  };

  vk::PipelineMultisampleStateCreateInfo ci_multisample{
    .rasterizationSamples = vk::SampleCountFlagBits::e1,
    .sampleShadingEnable  = false
  };

  vk::PipelineDepthStencilStateCreateInfo ci_depth{
    .depthTestEnable        = true,
    .depthWriteEnable       = true,
    .depthCompareOp         = vk::CompareOp::eLess,
    .depthBoundsTestEnable  = false,
    .stencilTestEnable      = false
  };

  vk::PipelineColorBlendAttachmentState colorAttachment{
    .blendEnable          = true,
    .srcColorBlendFactor  = vk::BlendFactor::eSrcAlpha,
    .dstColorBlendFactor  = vk::BlendFactor::eOneMinusSrcAlpha,
    .colorBlendOp         = vk::BlendOp::eAdd,
    .srcAlphaBlendFactor  = vk::BlendFactor::eOne,
    .dstAlphaBlendFactor  = vk::BlendFactor::eOneMinusSrcAlpha,
    .alphaBlendOp         = vk::BlendOp::eAdd,
    .colorWriteMask       = vk::ColorComponentFlagBits::eR |
                            vk::ColorComponentFlagBits::eG |
                            vk::ColorComponentFlagBits::eB |
                            vk::ColorComponentFlagBits::eA
  };

  vk::PipelineColorBlendStateCreateInfo ci_blend{
    .logicOpEnable    = false,
    .attachmentCount  = 1,
    .pAttachments     = &colorAttachment
  };

  vk::PipelineRenderingCreateInfo ci_rendering{
    .colorAttachmentCount     = 1,
    .pColorAttachmentFormats  = &ge_settings.format,
    .depthAttachmentFormat    = ge_settings.depth_format
  };

  gPipeline = Context::device().createGraphicsPipeline(nullptr, vk::GraphicsPipelineCreateInfo{
    .pNext                = &ci_rendering,
    .stageCount           = static_cast<unsigned int>(ci_stages.size()),
    .pStages              = ci_stages.data(),
    .pVertexInputState    = &ci_input,
    .pInputAssemblyState  = &ci_assembly,
    .pViewportState       = &ci_viewport,
    .pRasterizationState  = &ci_rasterizer,
    .pMultisampleState    = &ci_multisample,
    .pDepthStencilState   = &ci_depth,
    .pColorBlendState     = &ci_blend,
    .pDynamicState        = &ci_dynState,
    .layout               = layout
  });
}

Materials& Materials::instance() {
  if (p_materials == nullptr)
    p_materials = new Materials;

  return *p_materials;
}

void Materials::destroy() {
  if (p_materials == nullptr) return;

  delete p_materials;
  p_materials = nullptr;
}

void Materials::add(Material::MaterialBuilder builder) {
  builders.emplace_back(builder);
  materialMap.emplace(std::make_pair(builder.tag, Material()));
}

void Materials::load() {
  for (const auto& builder : builders)
    materialMap[builder.tag] = std::move(Material(builder));
}

} // namespace ge