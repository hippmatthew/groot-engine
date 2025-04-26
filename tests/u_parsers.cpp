#include "src/include/parsers.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE( "obj_parser", "[unit][parsers]" ) {
  auto [vertices, indices] = ge::ObjParser::parse("../tests/dat/quad.obj");

  std::vector<ge::Vertex> expectedVertices = {
    {{ -0.5, -0.5, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0, -1.0 }},
    {{ -0.5, 0.5, 0.0 }, { 0.0, 1.0 }, { 0.0, 0.0, -1.0 }},
    {{ 0.5, 0.5, 0.0 }, { 1.0, 1.0 }, { 0.0, 0.0, -1.0 }},
    {{ 0.5, -0.5, 0.0 }, { 1.0, 0.0 }, { 0.0, 0.0, -1.0 }}
  };

  std::vector<unsigned int> expectedIndices = { 0, 1, 2, 2, 3, 0 };

  REQUIRE( vertices.size() == expectedVertices.size() );
  CHECK( vertices == expectedVertices );

  REQUIRE( indices.size() == expectedIndices.size() );
  CHECK( indices == expectedIndices );
}