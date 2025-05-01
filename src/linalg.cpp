#include "src/include/linalg.hpp"

#include <cmath>
#include <compare>
#include <stdexcept>

namespace ge {

vec2::vec2(float s) {
  x = y = s;
}

vec2::vec2(float xval, float yval) {
  x = xval;
  y = yval;
}

vec2& vec2::operator+=(const vec2& rhs) {
  x += rhs.x;
  y += rhs.y;

  return *this;
}

vec2& vec2::operator-=(const vec2& rhs) {
  x -= rhs.x;
  y -= rhs.y;

  return *this;
}

vec2& vec2::operator*=(float rhs) {
  x *= rhs;
  y *= rhs;

  return *this;
}

vec2& vec2::operator/=(float rhs) {
  x /= rhs;
  y /= rhs;

  return *this;
}

float& vec2::operator[](unsigned int index) {
  switch (index) {
    case 0: return x;
    case 1: return y;
    default:
      throw std::out_of_range("groot-engine: vec2 index out of range");
  }
}

const float& vec2::operator[](unsigned int index) const {
  switch (index) {
    case 0: return x;
    case 1: return y;
    default:
      throw std::out_of_range("groot-engine: vec2 index out of range");
  }
}

std::partial_ordering vec2::operator<=>(const vec2& rhs) const {
  if (x < rhs.x && y < rhs.y)   return std::partial_ordering::less;
  if (x > rhs.x && x > rhs.y)   return std::partial_ordering::greater;
  if (x == rhs.x && y == rhs.y) return std::partial_ordering::equivalent;

  return std::partial_ordering::unordered;
}

vec2 vec2::operator+(const vec2& rhs) const {
  return vec2(x + rhs.x, y + rhs.y);
}

vec2 vec2::operator-(const vec2& rhs) const {
  return vec2(x - rhs.x, y - rhs.y);
}

vec2 vec2::operator-() const {
  return vec2(-x, -y);
}

float vec2::operator*(const vec2& rhs) const {
  return x * rhs.x + y * rhs.y;
}

vec2 vec2::operator*(float rhs) const {
  return vec2(rhs * x, rhs * y);
}

vec2 vec2::operator/(float rhs) const {
  return vec2(x / rhs, y / rhs);
}

float vec2::magnitude() const {
  return std::sqrt(*this * *this);
}

vec2 vec2::normalized() const {
  return *this / magnitude();
}

vec3::vec3(float s) {
  x = y = z = s;
}

vec3::vec3(float xval, float yval, float zval) {
  x = xval;
  y = yval;
  z = zval;
}

vec3::vec3(const vec2& v, float zval) {
  x = v.x;
  y = v.y;
  z = zval;
}

vec3& vec3::operator+=(const vec3& rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;

  return *this;
}

vec3& vec3::operator-=(const vec3& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;

  return *this;
}

vec3& vec3::operator*=(float rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;

  return *this;
}

vec3& vec3::operator/=(float rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;

  return *this;
}

float& vec3::operator[](unsigned int index) {
  switch (index) {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    default:
      throw std::out_of_range("groot-engine: vec3 index out of range");
  }
}

const float& vec3::operator[](unsigned int index) const {
  switch (index) {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    default:
      throw std::out_of_range("groot-engine: vec3 index out of range");
  }
}

std::partial_ordering vec3::operator<=>(const vec3& rhs) const {
  if (x < rhs.x && y < rhs.y && z < rhs.z)    return std::partial_ordering::less;
  if (x > rhs.x && x > rhs.y && z > rhs.z)    return std::partial_ordering::greater;
  if (x == rhs.x && y == rhs.y && z == rhs.z) return std::partial_ordering::equivalent;

  return std::partial_ordering::unordered;
}

vec3 vec3::operator+(const vec3& rhs) const {
  return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

vec3 vec3::operator-(const vec3& rhs) const {
  return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

vec3 vec3::operator-() const {
  return vec3(-x, -y, -z);
}

float vec3::operator*(const vec3& rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

vec3 vec3::operator*(float rhs) const {
  return vec3(rhs * x, rhs * y, rhs * z);
}

vec3 vec3::operator/(float rhs) const {
  return vec3(x / rhs, y / rhs, z / rhs);
}

vec2 vec3::xy() const {
  return vec2(x, y);
}

vec3 vec3::cross(const vec3& v) const {
  return vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

float vec3::magnitude() const {
  return std::sqrt(*this * *this);
}

vec3 vec3::normalized() const {
  return *this / magnitude();
}

vec4::vec4(float s) {
  x = y = z = w = s;
}

vec4::vec4(float xval, float yval, float zval, float wval) {
  x = xval;
  y = yval;
  z = zval;
  w = wval;
}

vec4::vec4(const vec2& v, float zval, float wval) {
  x = v.x;
  y = v.y;
  z = zval;
  w = wval;
}

vec4::vec4(const vec3& v, float wval) {
  x = v.x;
  y = v.y;
  z = v.z;
  w = wval;
}

vec4& vec4::operator+=(const vec4& rhs) {
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  w += rhs.w;

  return *this;
}

vec4& vec4::operator-=(const vec4& rhs) {
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  w -= rhs.w;

  return *this;
}

vec4& vec4::operator*=(float rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
  w *= rhs;

  return *this;
}

vec4& vec4::operator/=(float rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
  w /= rhs;

  return *this;
}

float& vec4::operator[](unsigned int index) {
  switch (index) {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    case 3: return w;
    default:
      throw std::out_of_range("groot-engine: vec4 index out of range");
  }
}

const float& vec4::operator[](unsigned int index) const {
  switch (index) {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    case 3: return w;
    default:
      throw std::out_of_range("groot-engine: vec4 index out of range");
  }
}

std::partial_ordering vec4::operator<=>(const vec4& rhs) const {
  if (x < rhs.x && y < rhs.y && z < rhs.z && w < rhs.w)     return std::partial_ordering::less;
  if (x > rhs.x && x > rhs.y && z > rhs.z && w > rhs.w)     return std::partial_ordering::greater;
  if (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w) return std::partial_ordering::equivalent;

  return std::partial_ordering::unordered;
}

vec4 vec4::operator+(const vec4& rhs) const {
  return vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

vec4 vec4::operator-(const vec4& rhs) const {
  return vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

vec4 vec4::operator-() const {
  return vec4(-x, -y, -z, -w);
}

float vec4::operator*(const vec4& rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
}

vec4 vec4::operator*(float rhs) const {
  return vec4(rhs * x, rhs * y, rhs * z, rhs * w);
}

vec4 vec4::operator/(float rhs) const {
  return vec4(x / rhs, y / rhs, z / rhs, w / rhs);
}

vec2 vec4::xy() const {
  return vec2(x, y);
}

vec3 vec4::xyz() const {
  return vec3(x, y, z);
}

float vec4::magnitude() const {
  return std::sqrt(*this * *this);
}

vec4 vec4::normalized() const {
  return *this / magnitude();
}

Quaternion::Quaternion(float w, float x, float y, float z) : real(w), imaginary(vec3(x, y, z)) {
  normalize();
}

Quaternion::Quaternion(float w, const vec3& v) : real(w), imaginary(v) {
  normalize();
}

Quaternion::Quaternion(const vec3& v) : imaginary(v) {
  normalize();
}

Quaternion& Quaternion::operator+=(const Quaternion& rhs) {
  *this = *this + rhs;
  return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& rhs) {
  *this = *this - rhs;
  return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
  *this = *this * rhs;
  return *this;
}

Quaternion& Quaternion::operator*=(float rhs) {
  *this = *this * rhs;
  return *this;
}

Quaternion& Quaternion::operator/=(float rhs) {
  *this = *this / rhs;
  return *this;
}

std::partial_ordering Quaternion::operator<=>(const Quaternion& rhs) const {
  if (real == rhs.real && imaginary == rhs.imaginary)
    return std::partial_ordering::equivalent;

  if (real < rhs.real && imaginary < rhs.imaginary)
    return std::partial_ordering::less;

  if (real > rhs.real && imaginary > rhs.imaginary)
    return std::partial_ordering::greater;

  return std::partial_ordering::unordered;
}

Quaternion Quaternion::operator+(const Quaternion& rhs) const {
  return Quaternion(real + rhs.real, imaginary + rhs.imaginary);
}

Quaternion Quaternion::operator-(const Quaternion& rhs) const {
  return Quaternion(real - rhs.real, imaginary - rhs.imaginary);
}

Quaternion Quaternion::operator-() const {
  return Quaternion(-real, -imaginary);
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const {
  return Quaternion(
    real * rhs.real - imaginary * rhs.imaginary,
    real * rhs.imaginary + rhs.real * imaginary - imaginary.cross(rhs.imaginary)
  );
}

Quaternion Quaternion::operator*(float rhs) const {
  return Quaternion(rhs * real, rhs * imaginary);
}

Quaternion Quaternion::operator/(float rhs) const {
  return Quaternion(real / rhs, imaginary / rhs);
}

Quaternion Quaternion::conjugate() const {
  return Quaternion(real, -imaginary);
}

void Quaternion::normalize() {
  float mag = std::sqrt(real * real + imaginary * imaginary);
  real /= mag;
  imaginary /= mag;
}

template <Layout layout>
mat2<layout>::mat2(float s) {
  m_rows[0] = m_rows[1] = vec2(s);
}

template <Layout layout>
mat2<layout>::mat2(const vec2& row1, const vec2& row2) {
  m_rows[0] = row1;
  m_rows[1] = row2;
}

template <Layout layout>
mat2<layout>::mat2(const mat3& m) {
  m_rows[0] = m[0].xy();
  m_rows[1] = m[1].xy();
}

template <Layout layout>
mat2<layout>::mat2(const mat4& m) {
  m_rows[0] = m[0].xy();
  m_rows[1] = m[1].xy();
}

template <Layout layout>
mat2<layout>& mat2<layout>::operator+=(const mat2& rhs) {
  for (unsigned int i = 0; i < 2; ++i)
    m_rows[i] += rhs.m_rows[i];

  return *this;
}

template <Layout layout>
mat2<layout>& mat2<layout>::operator-=(const mat2& rhs) {
  for (unsigned int i = 0; i < 2; ++i)
    m_rows[i] -= rhs.m_rows[i];

  return *this;
}

template <Layout layout>
mat2<layout>& mat2<layout>::operator*=(const mat2& rhs) {
  *this = *this * rhs;
  return *this;
}

template <Layout layout>
mat2<layout>& mat2<layout>::operator*=(float rhs) {
  for (unsigned int i = 0; i < 2; ++i)
    m_rows[i] *= rhs;

  return *this;
}

template <Layout layout>
mat2<layout>& mat2<layout>::operator/=(float rhs) {
  for (unsigned int i = 0; i < 2; ++i)
    m_rows[i] /= rhs;

  return *this;
}

template <Layout layout>
vec2& mat2<layout>::operator[](unsigned int index) {
  if (index > 1) throw std::out_of_range("groot-engine: mat2 index out of range");
  return m_rows[index];
}

template <Layout layout>
const vec2& mat2<layout>::operator[](unsigned int index) const {
  if (index > 1) throw std::out_of_range("groot-engine: mat2 index out of range");
  return m_rows[index];
}

template <Layout layout>
std::partial_ordering mat2<layout>::operator<=>(const mat2& rhs) const {
  bool less = true;
  bool greater = true;
  bool equivalent = true;

  for (unsigned int i = 0; i < 2; ++i) {
    less = less && (m_rows[i] < rhs.m_rows[i]);
    greater = greater && (m_rows[i] > rhs.m_rows[i]);
    equivalent = equivalent && (m_rows[i] == rhs.m_rows[i]);
  }

  if (less) return std::partial_ordering::less;
  if (greater) return std::partial_ordering::greater;
  if (equivalent) return std::partial_ordering::equivalent;

  return std::partial_ordering::unordered;
}

template <Layout layout>
mat2<layout> mat2<layout>::operator+(const mat2& rhs) const {
  return mat2(m_rows[0] + rhs.m_rows[0], m_rows[1] + rhs.m_rows[1]);
}

template <Layout layout>
mat2<layout> mat2<layout>::operator-(const mat2& rhs) const {
  return mat2(m_rows[0] - rhs.m_rows[0], m_rows[1] - rhs.m_rows[1]);
}

template <Layout layout>
mat2<layout> mat2<layout>::operator-() const {
  return mat2(-m_rows[0], -m_rows[1]);
}

template <Layout layout>
mat2<layout> mat2<layout>::operator*(const mat2& rhs) const {
  mat2 res(0.0f);

  for (unsigned int i = 0; i < 2; ++i) {
    for (unsigned int j = 0; j < 2; ++j) {
      res[i][j] = m_rows[i][0] * rhs.m_rows[0][j] +
                  m_rows[i][1] * rhs.m_rows[1][j];
    }
  }

  return res;
}

template <Layout layout>
vec2 mat2<layout>::operator*(const vec2& rhs) const {
  return vec2(m_rows[0] * rhs, m_rows[1] * rhs);
}

template <Layout layout>
mat2<layout> mat2<layout>::operator*(float rhs) const {
  return mat2(rhs * m_rows[0], rhs * m_rows[1]);
}

template <Layout layout>
mat2<layout> mat2<layout>::operator/(float rhs) const {
  return mat2(m_rows[0] / rhs, m_rows[1] / rhs);
}

template <Layout layout>
mat2<layout> mat2<layout>::identity() {
  return mat2(
    vec2(1.0f, 0.0f),
    vec2(0.0f, 1.0f)
  );
}

template <Layout layout>
mat2<layout> mat2<layout>::rotation(float angle) {
  if  (angle == 0) return identity();
  return mat2(
    vec2(std::cos(angle), -std::sin(angle)),
    vec2(std::sin(angle), std::cos(angle))
  );
}

template <Layout layout>
mat2<layout> mat2<layout>::scale(const vec2& scalar) {
  return mat2(
    vec2(scalar.x, 0.0f),
    vec2(0.0f, scalar.y)
  );
}

template <Layout layout>
float mat2<layout>::determinant() const {
  return m_rows[0][0] * m_rows[1][1] - m_rows[1][0] * m_rows[0][1];
}

template <Layout layout>
mat2<layout> mat2<layout>::transpose() const {
  return mat2(
    vec2(m_rows[0][0], m_rows[1][0]),
    vec2(m_rows[0][1], m_rows[1][1])
  );
}

template <Layout layout>
std::optional<mat2<layout>> mat2<layout>::inverse() const {
  float det = determinant();

  return !det ? std::nullopt : std::optional(mat2(
    vec2(m_rows[1][1], -m_rows[0][1]) / det,
    vec2(-m_rows[1][0], m_rows[0][0]) / det
  ));
}

mat3::mat3(float s) {
  m_rows[0] = m_rows[1] = m_rows[2] = vec3(s);
}

mat3::mat3(const vec3& row1, const vec3& row2, const vec3& row3) {
  m_rows[0] = row1;
  m_rows[1] = row2;
  m_rows[2] = row3;
}

mat3::mat3(const Quaternion& q) {
  const float &w = q.real, &x = q.imaginary.x, &y = q.imaginary.y, &z = q.imaginary.z;

  m_rows[0] = vec3(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + y * w));
  m_rows[1] = vec3(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
  m_rows[2] = vec3(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
}

template <Layout layout>
mat3::mat3(const mat2<layout>& m) {
  m_rows[0] = vec3(m[0], 0.0f);
  m_rows[1] = vec3(m[1], 0.0f);
  m_rows[2] = vec3(0.0f, 0.0f, 1.0f);
}

mat3::mat3(const mat4& m) {
  m_rows[0] = m[0].xyz();
  m_rows[1] = m[1].xyz();
  m_rows[2] = m[2].xyz();
}

mat3& mat3::operator+=(const mat3& rhs) {
  for (unsigned int i = 0; i < 3; ++i)
    m_rows[i] += rhs.m_rows[i];

  return *this;
}

mat3& mat3::operator-=(const mat3& rhs) {
  for (unsigned int i = 0; i < 3; ++i)
    m_rows[i] -= rhs.m_rows[i];

  return *this;
}

mat3& mat3::operator*=(const mat3& rhs) {
  *this = *this * rhs;
  return *this;
}

mat3& mat3::operator*=(float rhs) {
  for (unsigned int i = 0; i < 3; ++i)
    m_rows[i] *= rhs;

  return *this;
}

mat3& mat3::operator/=(float rhs) {
  for (unsigned int i = 0; i < 3; ++i)
    m_rows[i] /= rhs;

  return *this;
}

vec3& mat3::operator[](unsigned int index) {
  if (index > 2) throw std::out_of_range("groot-engine: mat2 index out of range");
  return m_rows[index];
}

const vec3& mat3::operator[](unsigned int index) const {
  if (index > 2) throw std::out_of_range("groot-engine: mat2 index out of range");
  return m_rows[index];
}

std::partial_ordering mat3::operator<=>(const mat3& rhs) const {
  bool less = true;
  bool greater = true;
  bool equivalent = true;

  for (unsigned int i = 0; i < 3; ++i) {
    less = less && (m_rows[i] < rhs.m_rows[i]);
    greater = greater && (m_rows[i] > rhs.m_rows[i]);
    equivalent = equivalent && (m_rows[i] == rhs.m_rows[i]);
  }

  if (less) return std::partial_ordering::less;
  if (greater) return std::partial_ordering::greater;
  if (equivalent) return std::partial_ordering::equivalent;

  return std::partial_ordering::unordered;
}

mat3 mat3::operator+(const mat3& rhs) const {
  return mat3(m_rows[0] + rhs.m_rows[0], m_rows[1] + rhs.m_rows[1], m_rows[2] + rhs.m_rows[2]);
}

mat3 mat3::operator-(const mat3& rhs) const {
  return mat3(m_rows[0] - rhs.m_rows[0], m_rows[1] - rhs.m_rows[1], m_rows[2] - rhs.m_rows[2]);
}

mat3 mat3::operator-() const {
  return mat3(-m_rows[0], -m_rows[1], -m_rows[2]);
}

mat3 mat3::operator*(const mat3& rhs) const {
  mat3 res(0.0f);

  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 3; ++j) {
      res[i][j] = m_rows[i][0] * rhs.m_rows[0][j] +
                  m_rows[i][1] * rhs.m_rows[1][j] +
                  m_rows[i][2] * rhs.m_rows[2][j];
    }
  }

  return res;
}

vec3 mat3::operator*(const vec3& rhs) const {
  return vec3(m_rows[0] * rhs, m_rows[1] * rhs, m_rows[2] * rhs);
}

mat3 mat3::operator*(float rhs) const {
  return mat3(rhs * m_rows[0], rhs * m_rows[1], rhs * m_rows[2]);
}

mat3 mat3::operator/(float rhs) const {
  return mat3(m_rows[0] / rhs, m_rows[1] / rhs, m_rows[2] / rhs);
}

mat3 mat3::identity() {
  return mat3(
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f)
  );
}

mat3 mat3::rotation(const vec3& rotator) {
  Quaternion qx = Quaternion(std::cos(rotator.x / 2), std::sin(rotator.x / 2), 0.0f, 0.0f);
  Quaternion qy = Quaternion(std::cos(rotator.y / 2), 0.0f, std::sin(rotator.y / 2), 0.0f);
  Quaternion qz = Quaternion(std::cos(rotator.z / 2), 0.0f, 0.0f, std::sin(rotator.z / 2));

  return ge::mat3(qy * qx * qz);
}

mat3 mat3::scale(const vec3& scalar) {
  return mat3(
    vec3(scalar.x, 0.0f, 0.0f),
    vec3(0.0f, scalar.y, 0.0f),
    vec3(0.0f, 0.0f, scalar.z)
  );
}

float mat3::determinant() const {
  const float &a = m_rows[0][0], &b = m_rows[0][1], &c = m_rows[0][2],
              &d = m_rows[1][0], &e = m_rows[1][1], &f = m_rows[1][2],
              &g = m_rows[2][0], &h = m_rows[2][1], &i = m_rows[2][2];

  return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

mat3 mat3::transpose() const {
  return mat3(
    vec3(m_rows[0][0], m_rows[1][0], m_rows[2][0]),
    vec3(m_rows[0][1], m_rows[1][1], m_rows[2][1]),
    vec3(m_rows[0][2], m_rows[1][2], m_rows[2][2])
  );
}

mat3 mat3::cofactor() const {
  const float  &a = m_rows[0][0], &b = m_rows[0][1], &c = m_rows[0][2],
                &d = m_rows[1][0], &e = m_rows[1][1], &f = m_rows[1][2],
                &g = m_rows[2][0], &h = m_rows[2][1], &i = m_rows[2][2];

  return mat3(
    vec3(e * i - f * h, f * g - d * i, d * h - e * g),
    vec3(c * h - b * i, a * i - c * g, b * g - a * h),
    vec3(b * f - c * e, c * d - a * f, a * e - b * d)
  );
}

mat3 mat3::adjugate() const {
  return cofactor().transpose();
}

std::optional<mat3> mat3::inverse() const {
  float det = determinant();
  return !det ? std::nullopt : std::optional(adjugate() / det);
}

mat4::mat4(float s) {
  m_rows[0] = m_rows[1] = m_rows[2] = m_rows[3] = vec4(s);
}

mat4::mat4(const vec4& row1, const vec4& row2, const vec4& row3, const vec4& row4) {
  m_rows[0] = row1;
  m_rows[1] = row2;
  m_rows[2] = row3;
  m_rows[3] = row4;
}

template <Layout layout>
mat4::mat4(const mat2<layout>& m) {
  m_rows[0] = vec4(m[0], 0.0f, 0.0f);
  m_rows[1] = vec4(m[1], 0.0f, 0.0f);
  m_rows[2] = vec4(0.0f, 0.0f, 1.0f, 0.0f);
  m_rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

mat4::mat4(const mat3& m) {
  m_rows[0] = vec4(m[0], 0.0f);
  m_rows[1] = vec4(m[1], 0.0f);
  m_rows[2] = vec4(m[2], 0.0f);
  m_rows[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

mat4& mat4::operator+=(const mat4& rhs) {
  for (unsigned int i = 0; i < 4; ++i)
    m_rows[i] += rhs.m_rows[i];

  return *this;
}

mat4& mat4::operator-=(const mat4& rhs) {
  for (unsigned int i = 0; i < 4; ++i)
    m_rows[i] -= rhs.m_rows[i];

  return *this;
}

mat4& mat4::operator*=(const mat4& rhs) {
  *this = *this * rhs;
  return *this;
}

mat4& mat4::operator*=(float rhs) {
  for (unsigned int i = 0; i < 4; ++i)
    m_rows[i] *= rhs;

  return *this;
}

mat4& mat4::operator/=(float rhs) {
  for (unsigned int i = 0; i < 4; ++i)
    m_rows[i] /= rhs;

  return *this;
}

vec4& mat4::operator[](unsigned int index) {
  if (index > 3) throw std::out_of_range("groot-engine: mat2 index out of range");
  return m_rows[index];
}

const vec4& mat4::operator[](unsigned int index) const {
  if (index > 3) throw std::out_of_range("groot-engine: mat2 index out of range");
  return m_rows[index];
}

std::partial_ordering mat4::operator<=>(const mat4& rhs) const {
  bool less = true;
  bool greater = true;
  bool equivalent = true;

  for (unsigned int i = 0; i < 4; ++i) {
    less = less && (m_rows[i] < rhs.m_rows[i]);
    greater = greater && (m_rows[i] > rhs.m_rows[i]);
    equivalent = equivalent && (m_rows[i] == rhs.m_rows[i]);
  }

  if (less) return std::partial_ordering::less;
  if (greater) return std::partial_ordering::greater;
  if (equivalent) return std::partial_ordering::equivalent;

  return std::partial_ordering::unordered;
}

mat4 mat4::operator+(const mat4& rhs) const {
  return mat4(m_rows[0] + rhs.m_rows[0], m_rows[1] + rhs.m_rows[1], m_rows[2] + rhs.m_rows[2], m_rows[3] + rhs.m_rows[3]);
}

mat4 mat4::operator-(const mat4& rhs) const {
  return mat4(m_rows[0] - rhs.m_rows[0], m_rows[1] - rhs.m_rows[1], m_rows[2] - rhs.m_rows[2], m_rows[3] - rhs.m_rows[3]);
}

mat4 mat4::operator-() const {
  return mat4(-m_rows[0], -m_rows[1], -m_rows[2], -m_rows[3]);
}

mat4 mat4::operator*(const mat4& rhs) const {
  mat4 res = mat4(0.0f);
  for (unsigned int i = 0; i < 4; ++i) {
    for (unsigned int j = 0; j < 4; ++j) {
      res[i][j] = m_rows[i][0] * rhs.m_rows[0][j] +
                  m_rows[i][1] * rhs.m_rows[1][j] +
                  m_rows[i][2] * rhs.m_rows[2][j] +
                  m_rows[i][3] * rhs.m_rows[3][j];
    }
  }

  return res;
}

vec4 mat4::operator*(const vec4& rhs) const {
  return vec4(m_rows[0] * rhs, m_rows[1] * rhs, m_rows[2] * rhs, m_rows[3] * rhs);
}

mat4 mat4::operator*(float rhs) const {
  return mat4(rhs * m_rows[0], rhs * m_rows[1], rhs * m_rows[2], rhs * m_rows[3]);
}

mat4 mat4::operator/(float rhs) const {
  return mat4(m_rows[0] / rhs, m_rows[1] / rhs, m_rows[2] / rhs, m_rows[3] / rhs);
}

mat4 mat4::identity() {
  return mat4(
    vec4(1.0f, 0.0f, 0.0f, 0.0f),
    vec4(0.0f, 1.0f, 0.0f, 0.0f),
    vec4(0.0f, 0.0f, 1.0f, 0.0f),
    vec4(0.0f, 0.0f, 0.0f, 1.0f)
  );
}

mat4 mat4::rotation(const vec3& rotator) {
  if (rotator == vec3(0.0f)) return mat4::identity();
  return mat4(mat3::rotation(rotator));
}

mat4 mat4::scale(const vec3& scalar) {
  return mat4(mat3::scale(scalar));
}

mat4 mat4::translation(const vec3& translator) {
  return mat4(
    vec4(1.0f, 0.0f, 0.0f, translator.x),
    vec4(0.0f, 1.0f, 0.0f, translator.y),
    vec4(0.0f, 0.0f, 1.0f, translator.z),
    vec4(0.0f, 0.0f, 0.0f, 1.0f)
  );
}

mat4 mat4::view(const vec3& eye, const vec3& target, const vec3& up) {
  vec3 w = (target - eye).normalized();
  vec3 u = w.cross(up).normalized();
  vec3 v = w.cross(u);

  return mat4(
    vec4(u, -u * eye),
    vec4(v, -v * eye),
    vec4(w, -w * eye),
    vec4(0.0f, 0.0f, 0.0f, 1.0f)
  );
}

mat4 mat4::perspective(float fov, float ar, float near, float far) {
  float taninv = 1 / std::tan(fov / 2);
  float fmninv = 1 / (far - near);

  return mat4(
    vec4(taninv / ar, 0.0f, 0.0f, 0.0f),
    vec4(0.0f, taninv, 0.0f, 0.0f),
    vec4(0.0f, 0.0f, far * fmninv, -far * near * fmninv),
    vec4(0.0f, 0.0f, 1.0f, 0.0f)
  );
}

float mat4::determinant() const {
  mat4 cof = cofactor();

  float det = 0.0f;
  for (unsigned int j = 0; j < 4; ++j)
    det += m_rows[0][j] * cof[0][j];

  return det;
}

mat4 mat4::transpose() const {
  return mat4(
    vec4(m_rows[0][0], m_rows[1][0], m_rows[2][0], m_rows[3][0]),
    vec4(m_rows[0][1], m_rows[1][1], m_rows[2][1], m_rows[3][1]),
    vec4(m_rows[0][2], m_rows[1][2], m_rows[2][2], m_rows[3][2]),
    vec4(m_rows[0][3], m_rows[3][1], m_rows[2][3], m_rows[3][3])
  );
}

mat4 mat4::cofactor() const {
  const float &a = m_rows[0][0], &b = m_rows[0][1], &c = m_rows[0][2], &d = m_rows[0][3],
              &e = m_rows[1][0], &f = m_rows[1][1], &g = m_rows[1][2], &h = m_rows[1][3],
              &i = m_rows[2][0], &j = m_rows[2][1], &k = m_rows[2][2], &l = m_rows[2][3],
              &m = m_rows[3][0], &n = m_rows[3][1], &o = m_rows[3][2], &p = m_rows[3][3];

  mat3 m00 = mat3(
    vec3(f, g, h),
    vec3(j, k, l),
    vec3(n, o, p)
  );

  mat3 m01 = mat3(
    vec3(e, g, h),
    vec3(i, k, l),
    vec3(m, o, p)
  );

  mat3 m02 = mat3(
    vec3(e, f, h),
    vec3(i, j, l),
    vec3(m, n, p)
  );

  mat3 m03 = mat3(
    vec3(e, f, g),
    vec3(i, j, k),
    vec3(m, n, o)
  );

  mat3 m10 = mat3(
    vec3(b, c, d),
    vec3(j, k, l),
    vec3(n, o, p)
  );

  mat3 m11 = mat3(
    vec3(a, c, d),
    vec3(i, k, l),
    vec3(m, o, p)
  );

  mat3 m12 = mat3(
    vec3(a, b, d),
    vec3(i, j, l),
    vec3(m, n, p)
  );

  mat3 m13 = mat3(
    vec3(a, b, c),
    vec3(i, j, k),
    vec3(m, n, o)
  );

  mat3 m20 = mat3(
    vec3(b, c, d),
    vec3(f, g, h),
    vec3(n, o, p)
  );

  mat3 m21 = mat3(
    vec3(a, c, d),
    vec3(e, g, h),
    vec3(m, o, p)
  );

  mat3 m22 = mat3(
    vec3(a, b, d),
    vec3(e, f, h),
    vec3(m, n, p)
  );

  mat3 m23 = mat3(
    vec3(a, b, c),
    vec3(e, f, g),
    vec3(m, n, o)
  );

  mat3 m30 = mat3(
    vec3(b, c, d),
    vec3(f, g, h),
    vec3(j, k, l)
  );

  mat3 m31 = mat3(
    vec3(a, c, d),
    vec3(e, g, h),
    vec3(i, k, l)
  );

  mat3 m32 = mat3(
    vec3(a, b, d),
    vec3(e, f, h),
    vec3(i, j, l)
  );

  mat3 m33 = mat3(
    vec3(a, b, c),
    vec3(e, f, g),
    vec3(i, j, k)
  );

  return mat4(
    vec4(m00.determinant(), -m01.determinant(), m02.determinant(), -m03.determinant()),
    vec4(-m10.determinant(), m11.determinant(), -m12.determinant(), m13.determinant()),
    vec4(m20.determinant(), -m21.determinant(), m22.determinant(), -m23.determinant()),
    vec4(-m30.determinant(), m31.determinant(), -m32.determinant(), m33.determinant())
  );
}

mat4 mat4::adjugate() const {
  return cofactor().transpose();
}

std::optional<mat4> mat4::inverse() const {
  float det = determinant();
  return !det ? std::nullopt : std::optional(adjugate() / det);
}

} // namespace ge

ge::vec2 operator*(float lhs, const ge::vec2& rhs) {
  return rhs * lhs;
}

ge::vec3 operator*(float lhs, const ge::vec3& rhs) {
  return rhs * lhs;
}

ge::vec4 operator*(float lhs, const ge::vec4& rhs) {
  return rhs * lhs;
}

ge::Quaternion operator*(float lhs, const ge::Quaternion& rhs) {
  return rhs * lhs;
}

template <ge::Layout layout>
ge::mat2<layout> operator*(float lhs, const ge::mat2<layout>& rhs) {
  return rhs * lhs;
}

ge::mat3 operator*(float lhs, const ge::mat3& rhs) {
  return rhs * lhs;
}

ge::mat4 operator*(float lhs, const ge::mat4& rhs) {
  return rhs * lhs;
}

template class ge::mat2<ge::Layout::std430>;
template class ge::mat2<ge::Layout::std140>;