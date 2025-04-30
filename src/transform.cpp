#include "src/include/transform.hpp"

namespace ge {

Transform::Transform(const vec3& p, const vec3& r, const vec3& s) : m_position(p), m_rotation(r), m_scale(s) {}

const vec3& Transform::position() const {
  return m_position;
}

const vec3& Transform::rotation() const {
  return m_rotation;
}

const vec3& Transform::scale() const {
  return m_scale;
}

Transform& Transform::translate(const vec3& p) {
  m_position = p;
  return *this;
}

Transform& Transform::rotate(const vec3& r) {
  m_rotation = r;
  return *this;
}

Transform& Transform::resize(const vec3& s) {
  m_scale = s;
  return *this;
}

} // namespace ge