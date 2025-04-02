#define ge_tests
#include "src/include/materials.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE( "materials", "[unit][materials]" ) {
  ge_materials.add(ge::Material::builder("test")
    .add_shader(ge::ShaderType::ComputeShader, "test/compute/path")
    .add_shader(ge::ShaderType::VertexShader, "test/vertex/path")
    .add_shader(ge::ShaderType::FragmentShader, "test/fragment/path")
  );

  auto builders = ge_materials.get_builders();
  REQUIRE( builders.size() == 1 );

  CHECK( builders[0].get_tag() == "test" );
  CHECK( builders[0].get_shaders().size() == 3 );

  unsigned int stages= 0x000;
  for (auto [stage, path] : builders[0].get_shaders()) {
    if (stage == vk::ShaderStageFlagBits::eCompute && path == "test/compute/path")
      stages |= 0x100;
    else if (stage == vk::ShaderStageFlagBits::eVertex && path == "test/vertex/path")
      stages |= 0x010;
    else if (stage == vk::ShaderStageFlagBits::eFragment && path == "test/fragment/path")
      stages |= 0x001;
  }

  CHECK( (stages & 0x111) == 0x111 );
}