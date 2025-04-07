#define ge_tests
#include "src/include/materials.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE( "materials", "[unit][materials]" ) {
  ge_materials.add("test", ge_material_builder
    .add_shader(ge::ShaderType::ComputeShader, "test")
    .add_shader(ge::ShaderType::VertexShader, "test")
    .add_shader(ge::ShaderType::FragmentShader, "test")
  );

  REQUIRE( ge_materials.get_builders().contains("test") );

  auto& builder = ge_materials.get_builders().at("test");
  CHECK( builder.get_shaders().contains(vk::ShaderStageFlagBits::eCompute) );
  CHECK( builder.get_shaders().contains(vk::ShaderStageFlagBits::eVertex) );
  CHECK( builder.get_shaders().contains(vk::ShaderStageFlagBits::eFragment) );
}