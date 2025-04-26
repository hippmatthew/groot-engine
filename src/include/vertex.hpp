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
    Vertex(vec<3>, vec<2>, vec<3>);

    ~Vertex() = default;

    Vertex& operator = (const Vertex&) = default;
    Vertex& operator = (Vertex&&) = default;

    static vk::VertexInputBindingDescription binding();
    static std::array<vk::VertexInputAttributeDescription, 3> attributes();

    bool operator == (const Vertex&) const;

  public:
    vec<3> position = vec<3>::zero();
    vec<2> uv = vec<2>::zero();
    vec<3> normal = vec<3>::zero();
};

} // namespace ge