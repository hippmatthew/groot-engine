project(GrootEngine::tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Catch2 3 REQUIRED)

set(TESTS_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/u_engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/u_linalg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/u_parsers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/utility.hpp
)

add_executable(test ${TESTS_SOURCES})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
  set_target_properties(test PROPERTIES LINK_FLAGS "-rpath /usr/local/lib")
endif()

target_link_libraries(test PRIVATE
  geng
  Catch2::Catch2
)