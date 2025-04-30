#include "src/include/vertex.hpp"

#include <__stddef_offsetof.h>

namespace ge {

Vertex::Vertex(vec3 v, vec2 u, vec3 n) : m_position(v), m_uv(u), m_normal(n) {}

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
      .offset   = offsetof(Vertex, m_position)
    },
    vk::VertexInputAttributeDescription{
      .location = 1,
      .binding  = 0,
      .format   = vk::Format::eR32G32Sfloat,
      .offset   = offsetof(Vertex, m_uv)
    },
    vk::VertexInputAttributeDescription{
      .location = 2,
      .binding  = 0,
      .format   = vk::Format::eR32G32B32Sfloat,
      .offset   = offsetof(Vertex, m_normal)
    }
  };
}

bool Vertex::operator==(const Vertex& rhs) const {
  return m_position == rhs.m_position && m_uv == rhs.m_uv && m_normal == rhs.m_normal;
}

} // namespace ge