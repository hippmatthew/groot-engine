#define ge_tests
#include "src/include/materials.hpp"
#include "src/include/objects.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE( "objects", "[unit][objects]" ) {
  ge::Materials::destroy();

  SECTION( "material_exception" ) {
    bool caught = false;
    try {
      ge_objects.add("test", "../tests/dat/quad.obj");
    }
    catch (const std::runtime_error& e) {
      caught = true;
      CHECK( e.what() == std::string("groot-engine: material 'test' not found") );
    }
    REQUIRE( caught );
  }

  SECTION( "add_object" ) {
    ge_materials.add("test", ge_material_builder);
    ge_objects.add("test", "../tests/dat/quad.obj");

    auto objects = ge_objects.get_objects();
    REQUIRE( objects.contains("test") );
    CHECK( objects["test"].vertices.size() == 4 );
    CHECK( objects["test"].indices.size() == 6 );

    auto [vIdx, iIdx, inIdx] = objects["test"].bufIndices;
    CHECK( vIdx == 0 );
    CHECK( iIdx == 0 );
    CHECK( inIdx == 0 );

    CHECK( objects["test"].commands.size() == 1 );
  }
}