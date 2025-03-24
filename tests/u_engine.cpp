#include "src/include/context.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE( "engine" ) {
  ge::Context context;
  context.run();
}