#include "src/include/engine.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE( "engine", "[unit][engine]" ) {
  ge::Engine engine;

  bool success = true;
  try {
    engine.run();
  }
  catch (std::exception _) {
    success = false;
  }

  CHECK( success );
}