#include "src/include/engine.hpp"

#include <catch2/catch_test_macros.hpp>

#include <chrono>
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

    ge::Transform& obj1 = engine.add_object("test", "../tests/dat/circle.obj",
      ge::Transform(ge::vec3(0.0f), ge::vec3(0.0f), ge::vec3(0.8f, 0.8f, 0.8f))
    );

    ge::Transform& obj2 = engine.add_object("test", "../tests/dat/quad.obj",
      ge::Transform(ge::vec3(-2.0f, 0.0f, 1.0f), ge::vec3(0.0f, std::numbers::pi / 8, 0.0f), ge::vec3(1.0f))
    );

    ge::Transform& obj3 = engine.add_object("test", "../tests/dat/pentagon.obj");
    obj3 = ge::Transform(ge::vec3(2.0f, 0.0f, 1.0f), ge::vec3(0.0f), ge::vec3(0.8f, 0.8f, 1.0f));

    std::chrono::time_point currTime = std::chrono::steady_clock::now();
    std::chrono::time_point prevTime = currTime;
    float t = 0.0f;

    bool success = true;
    try {
      engine.run([&obj1, &obj2, &obj3, &currTime, &prevTime, &t]() {
        currTime = std::chrono::steady_clock::now();
        t = std::chrono::duration(currTime - prevTime).count();
        prevTime = currTime;

        obj1.rotate(ge::vec3(0.0f, 0.0f, -0.01 * t));
      });
    }
    catch (const std::exception& e) {
      success = false;
    }
    CHECK( success );
  }
}