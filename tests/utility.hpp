#pragma once

#include "src/include/linalg.hpp"

#include <random>

namespace tests {

inline float g_absTolerance = 1e-6;
inline float g_relTolerance = 1e-4;

class Random {
  public:
    Random() = default;
    Random(Random&) = delete;
    Random(Random&&) = delete;

    ~Random() = default;

    Random& operator = (Random&) = delete;
    Random& operator = (Random&&) = delete;

    float operator () () {
      return distribution(generator);
    }

  private:
    std::random_device device;
    std::mt19937_64 generator = std::mt19937_64(device());
    std::uniform_real_distribution<float> distribution = std::uniform_real_distribution<float>(-50.0f, 50.0f);
};

inline float error(float res, float exp) {
  return std::abs(res - exp);
}

inline float error(const ge::vec2& res, const ge::vec2& exp) {
  return (res - exp).magnitude();
}

inline float error(const ge::vec3& res, const ge::vec3& exp) {
  return (res - exp).magnitude();
}

inline float error(const ge::vec4& res, const ge::vec4& exp) {
  return (res - exp).magnitude();
}

template <ge::Layout layout>
inline float error(const ge::mat2<layout>& res, const ge::mat2<layout>& exp) {
  ge::mat2<layout> delta = res - exp;

  float sum = 0.0f;
  for (unsigned int i = 0; i < 2; ++i) {
    for (unsigned int j = 0; j < 2; ++j)
      sum += delta[i][j] * delta[i][j];
  }

  return std::sqrt(sum);
}

inline float error(const ge::mat3& res, const ge::mat3& exp) {
  ge::mat3 delta = res - exp;

  float sum = 0.0f;
  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 3; ++j)
      sum += delta[i][j] * delta[i][j];
  }

  return std::sqrt(sum);
}

inline float error(const ge::mat4& res, const ge::mat4& exp) {
  ge::mat4 delta = res - exp;

  float sum = 0.0f;
  for (unsigned int i = 0; i < 4; ++i) {
    for (unsigned int j = 0; j < 4; ++j)
      sum += delta[i][j] * delta[i][j];
  }

  return std::sqrt(sum);
}

inline float tolerance(const float& ref) {
  return g_absTolerance + g_relTolerance * std::abs(ref);
}

inline float tolerance(const ge::vec2& ref) {
  return g_absTolerance + g_relTolerance * ref.magnitude();
}

inline float tolerance(const ge::vec3& ref) {
  return g_absTolerance + g_relTolerance * ref.magnitude();
}

inline float tolerance(const ge::vec4& ref) {
  return g_absTolerance + g_relTolerance * ref.magnitude();
}

template <ge::Layout layout>
inline float tolerance(const ge::mat2<layout>& ref) {
  float sum = 0.0f;
  for (unsigned int i = 0; i < 2; ++i) {
    for (unsigned int j = 0; j < 2; ++j)
      sum += ref[i][j] * ref[i][j];
  }

  return g_absTolerance + g_relTolerance * std::sqrt(sum);
}

inline float tolerance(const ge::mat3& ref) {
float sum = 0.0f;
  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 3; ++j)
      sum += ref[i][j] * ref[i][j];
  }

  return g_absTolerance + g_relTolerance * std::sqrt(sum);
}

inline float tolerance(const ge::mat4& ref) {
  float sum = 0.0f;
  for (unsigned int i = 0; i < 4; ++i) {
    for (unsigned int j = 0; j < 4; ++j)
      sum += ref[i][j] * ref[i][j];
  }

  return g_absTolerance + g_relTolerance * std::sqrt(sum);
}

} // namespace tests