#pragma once

#include "src/include/linalg.hpp"

namespace ge {

class Transform {
  public:
    Transform() = default;
    Transform(const Transform&) = default;
    Transform(Transform&&) = default;
    Transform(const vec3&, const vec3&, const vec3&);

    ~Transform() = default;

    Transform& operator=(const Transform&) = default;
    Transform& operator=(Transform&&) = default;

    const vec3& position() const;
    const vec3& rotation() const;
    const vec3& scale() const;

    Transform& translate(const vec3&);
    Transform& rotate(const vec3&);
    Transform& resize(const vec3&);

  private:
    vec3 m_position = vec3(0.0f);
    vec3 m_rotation = vec3(0.0f);
    vec3 m_scale = vec3(1.0f);
};

} // namespace ge