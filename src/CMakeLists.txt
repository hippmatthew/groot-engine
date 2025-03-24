project(GrootEngine::geng)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(GE_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/allocator.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/context.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/linalg.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parsers.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/settings.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/vertex.hpp
)

set(GE_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/allocator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/context.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/linalg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/parsers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/settings.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/vertex.cpp
)

add_library(geng SHARED ${GE_INCLUDES} ${GE_SOURCES})

target_include_directories(geng PUBLIC
  ${CMAKE_SOURCE_DIR}
  /usr/local/include
)

target_link_directories(geng PUBLIC
  /usr/local/lib
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
  target_include_directories(geng PUBLIC /opt/homebrew/include)
  target_link_directories(geng PUBLIC /opt/homebrew/lib)
endif()

target_compile_definitions(geng PRIVATE
  "GLFW_INCLUDE_NONE"
  "VULKAN_HPP_NO_CONSTRUCTORS"
  "ge_engine_version=ge_make_version(${GE_VERSION_MAJOR}, ${GE_VERSION_MINOR}, ${GE_VERSION_PATCH})"
  "ge_vulkan_version=VK_MAKE_API_VERSION(1, 3, 296, 0)"
)

target_link_libraries(geng PRIVATE
  Vulkan::Vulkan
  glfw
)