#pragma once

#include "src/include/linalg.hpp"

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vulkan_beta.h>

namespace ge {

class Vertex {
  public:
    Vertex() = default;
    Vertex(const Vertex&) = default;
    Vertex(Vertex&&) = default;
    Vertex(vec3, vec2, vec3);

    ~Vertex() = default;

    Vertex& operator = (const Vertex&) = default;
    Vertex& operator = (Vertex&&) = default;

    static vk::VertexInputBindingDescription binding();
    static std::array<vk::VertexInputAttributeDescription, 3> attributes();

    bool operator == (const Vertex&) const;

  public:
    vec3 m_position = vec3(0.0f);
    vec2 m_uv = vec2(0.0f);
    vec3 m_normal = vec3(0.0f);
};

} // namespace ge