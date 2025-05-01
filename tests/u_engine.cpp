#include "src/include/engine.hpp"

#include <catch2/catch_test_macros.hpp>

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

    ge::transform obj1 = engine.add_object("test", "../tests/dat/circle.obj",
      ge::Transform(ge::vec3(0.0f, 0.0f, 3.0f), ge::vec3(0.0f), ge::vec3(0.8f, 0.8f, 0.8f))
    );

    ge::transform obj2 = engine.add_object("test", "../tests/dat/quad.obj",
      ge::Transform(ge::vec3(0.0f, 0.0f, 1.5f), ge::vec3(0.0f), ge::vec3(1.0f))
    );

    ge::transform obj3 = engine.add_object("test", "../tests/dat/pentagon.obj",
      ge::Transform(ge::vec3(3.0f, 0.0f, 2.0f), ge::vec3(0.0f), ge::vec3(0.74f))
    );

    float w = 0.05;
    float a = 1.5f;
    float av = ge::radians(40.0f);

    bool success = true;
    try {
      engine.run([&obj1, &obj2, &obj3, &w, &a, &av](double dt) {
        float x1 = w * a * std::cos(w * obj1->elapsed_time()) * dt;
        float z1 = w * a * std::sin(w * obj1->elapsed_time()) * dt;

        float x2 = w * a * std::cos(w * obj2->elapsed_time()) * dt;
        float y2 = w * a * std::sin(w * obj2->elapsed_time()) * dt;

        float y3 = w * a * std::cos(w * obj3->elapsed_time()) * dt;
        float z3 = w * a * std::sin(w * obj3->elapsed_time()) * dt;

        obj1->translate(ge::vec3(x1, 0.0f, z1));
        obj1->rotate(ge::vec3(0.0f, 0.0f, -av * dt));

        obj2->translate(ge::vec3(x2, y2, 0.0f));
        obj2->rotate(ge::vec3(0.0f, 0.0f, -av * dt));

        obj3->translate(ge::vec3(0.0f, y3, z3));
        obj3->rotate(ge::vec3(0.0f, 0.0f, -av * dt));
      });
    }
    catch (const std::exception& e) {
      success = false;
    }
    CHECK( success );
  }
}