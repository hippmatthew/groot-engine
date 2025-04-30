#include "src/include/engine.hpp"

#include <catch2/catch_test_macros.hpp>

#include <numbers>

TEST_CASE( "engine", "[unit][engine]" ) {
  ge::Engine engine;

  SECTION( "add_duplicate_material" ) {
    engine.add_material("test", ge::MaterialManager::Builder());

    bool caught = false;
    try {
      engine.add_material("test", ge::MaterialManager::Builder());
    }
    catch (const std::runtime_error& e) {
      caught = true;
      CHECK( e.what() == std::string("groot-engine: material 'test' already exists") );
    }
    CHECK( caught );
  }

  SECTION( "object_with_invalid_material" ) {
    bool caught = false;
    try {
      engine.add_object("test", "../tests/dat/quad.obj");
    }
    catch (const std::runtime_error& e) {
      caught = true;
      CHECK( e.what() == std::string("groot-engine: material 'test' does not exist"));
    }
    CHECK( caught );
  }

  SECTION( "full_test" ) {
    engine.add_material("test", ge::MaterialManager::Builder()
      .add_shader(ge::ShaderStage::VertexShader, "shaders/shader.vert.spv")
      .add_shader(ge::ShaderStage::FragmentShader, "shaders/shader.frag.spv")
    );


    ge::transform obj1 = engine.add_object("test", "../tests/dat/quad.obj",
      ge::Transform(ge::vec3(0.0f), ge::vec3(0.0f), ge::vec3(0.8f, 0.8f, 0.8f))
    );

    ge::transform obj2 = engine.add_object("test", "../tests/dat/quad.obj",
      ge::Transform(ge::vec3(-2.0f, 0.0f, 1.0f), ge::vec3(0.0f, std::numbers::pi / 8, 0.0f), ge::vec3(1.0f))
    );

    ge::transform obj3 = engine.add_object("test", "../tests/dat/quad.obj",
      ge::Transform(ge::vec3(2.0f, 0.0f, 1.0f), ge::vec3(0.0f, -std::numbers::pi / 8, 0.0f), ge::vec3(1.0f))
    );

    bool success = true;
    try {
      engine.run();
    }
    catch (const std::exception& e) {
      success = false;
    }
    CHECK( success );
  }
}