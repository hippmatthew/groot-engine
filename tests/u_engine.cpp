#define ge_tests
#include "src/include/context.hpp"
#include "src/include/materials.hpp"
#include "src/include/objects.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE( "engine" ) {
  ge::Materials::destroy();
  ge::Objects::destroy();

  ge::Context context;

  REQUIRE( context.get_window() != nullptr );
  REQUIRE( *context.get_instance() != nullptr );
  REQUIRE( *context.get_gpu() != nullptr );
  REQUIRE( *context.get_device() != nullptr );
  REQUIRE( !context.get_queues().empty() );
  REQUIRE( context.get_queues().contains(ge::QueueFamilyType::Main) );

  ge_materials.add(ge::Material::builder("test")
    .add_shader(ge::ShaderType::FragmentShader, "shaders/shader.frag.spv")
    .add_shader(ge::ShaderType::VertexShader, "shaders/shader.vert.spv")
  );

  ge_objects.add("test", "../tests/dat/quad.obj");
  ge_objects.add("test", "../tests/dat/quad.obj");
  ge_objects.add("test", "../tests/dat/quad.obj");

  bool firstFrame = true;
  context.run([&firstFrame](){
    if (!firstFrame) return;

    auto& material = ge_materials.get_map().at("test");

    REQUIRE( *material.get_layout() != nullptr );
    REQUIRE( *material.get_gPipeline() != nullptr );
    REQUIRE( *ge_objects.get_vMem() != nullptr );
    REQUIRE( ge_objects.get_vBufs().size() == 1 );
    REQUIRE( *ge_objects.get_iMem() != nullptr );
    REQUIRE( ge_objects.get_iBufs().size() == 1 );
    REQUIRE( *ge_objects.get_inMem() != nullptr );
    REQUIRE( ge_objects.get_inBufs().size() == 1 );

    firstFrame = false;
  });
}