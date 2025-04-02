project(GrootEngine::tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Catch2 3 REQUIRED)
find_program(GLSLC glslc required)

set(TESTS_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/u_engine.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/u_linalg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/u_materials.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/u_objects.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/u_parsers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/utility.hpp
)

set(TESTS_SHADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.frag
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shader.vert
)

foreach(SHADER ${TESTS_SHADERS})
  get_filename_component(FILE_NAME ${SHADER} NAME)
  set(SPV ${CMAKE_BINARY_DIR}/shaders/${FILE_NAME}.spv)

  add_custom_command(OUTPUT ${SPV}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/shaders
    COMMAND ${GLSLC} -o ${SPV} ${SHADER}
    DEPENDS ${SHADER}
    COMMENT "compiling ${SHADER}..."
  )

  list(APPEND SPVS ${SPV})
endforeach()

add_custom_target(shaders ALL DEPENDS ${SPVS})
add_executable(test ${TESTS_SOURCES})
add_dependencies(test shaders)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
  set_target_properties(test PROPERTIES LINK_FLAGS "-rpath /usr/local/lib")
endif()

target_link_libraries(test PRIVATE
  geng
  Catch2::Catch2
)

