#include "src/include/vertex.hpp"

#include <__stddef_offsetof.h>

namespace ge {

Vertex::Vertex(vec<3> v, vec<2> u, vec<3> n) : position(v), uv(u), normal(n) {}

vk::VertexInputBindingDescription Vertex::binding() {
  return vk::VertexInputBindingDescription{
    .binding    = 0,
    .stride     = sizeof(Vertex),
    .inputRate  = vk::VertexInputRate::eVertex
  };
}

std::array<vk::VertexInputAttributeDescription, 3> Vertex::attributes() {
  return std::array<vk::VertexInputAttributeDescription, 3>{
    vk::VertexInputAttributeDescription{
      .location = 0,
      .binding  = 0,
      .format   = vk::Format::eR32G32B32Sfloat,
      .offset   = offsetof(Vertex, position)
    },
    vk::VertexInputAttributeDescription{
      .location = 1,
      .binding  = 0,
      .format   = vk::Format::eR32G32Sfloat,
      .offset   = offsetof(Vertex, uv)
    },
    vk::VertexInputAttributeDescription{
      .location = 2,
      .binding  = 0,
      .format   = vk::Format::eR32G32B32Sfloat,
      .offset   = offsetof(Vertex, normal)
    }
  };
}

bool Vertex::operator == (const Vertex& rhs) const {
  return position == rhs.position && uv == rhs.uv && normal == rhs.normal;
}

} // namespace ge