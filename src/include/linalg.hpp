#pragma once

#include <compare>
#include <optional>

namespace ge {

class vec3;
class vec4;
class mat3;
class mat4;

enum Layout {
  std140,
  std430
};

class alignas(8) vec2 {
  public:
    vec2() = delete;
    vec2(const vec2&) = default;
    vec2(vec2&&) = default;
    explicit vec2(float);
    vec2(float, float);

    ~vec2() = default;

    vec2& operator=(const vec2&) = default;
    vec2& operator=(vec2&&) = default;
    vec2& operator+=(const vec2&);
    vec2& operator-=(const vec2&);
    vec2& operator*=(float);
    vec2& operator/=(float);

    float& operator[](unsigned int);
    const float& operator[](unsigned int) const;

    std::partial_ordering operator<=>(const vec2&) const;
    bool operator==(const vec2&) const = default;

    vec2 operator+(const vec2&) const;
    vec2 operator-(const vec2&) const;
    vec2 operator-() const;
    float operator*(const vec2&) const;
    vec2 operator*(float) const;
    vec2 operator/(float) const;

    float magnitude() const;
    vec2 normalized() const;

  public:
    float x, y;
};

class alignas(16) vec3 {
  public:
    vec3() = delete;
    vec3(const vec3&) = default;
    vec3(vec3&&) = default;
    explicit vec3(float);
    vec3(float, float, float);
    vec3(const vec2&, float);

    ~vec3() = default;

    vec3& operator=(const vec3&) = default;
    vec3& operator=(vec3&&) = default;
    vec3& operator+=(const vec3&);
    vec3& operator-=(const vec3&);
    vec3& operator*=(float);
    vec3& operator/=(float);

    float& operator[](unsigned int);
    const float& operator[](unsigned int) const;

    std::partial_ordering operator<=>(const vec3&) const;
    bool operator==(const vec3&) const = default;

    vec3 operator+(const vec3&) const;
    vec3 operator-(const vec3&) const;
    vec3 operator-() const;
    float operator*(const vec3&) const;
    vec3 operator*(float) const;
    vec3 operator/(float) const;

    vec2 xy() const;
    vec3 cross(const vec3&) const;
    float magnitude() const;
    vec3 normalized() const;

  public:
    float x, y, z;
};

class alignas(16) vec4 {
  public:
    vec4() = delete;
    vec4(const vec4&) = default;
    vec4(vec4&&) = default;
    explicit vec4(float);
    vec4(float, float, float, float);
    vec4(const vec2&, float, float);
    vec4(const vec3&, float);

    ~vec4() = default;

    vec4& operator=(const vec4&) = default;
    vec4& operator=(vec4&&) = default;
    vec4& operator+=(const vec4&);
    vec4& operator-=(const vec4&);
    vec4& operator*=(float);
    vec4& operator/=(float);

    float& operator[](unsigned int);
    const float& operator[](unsigned int) const;

    std::partial_ordering operator<=>(const vec4&) const;
    bool operator==(const vec4&) const = default;

    vec4 operator+(const vec4&) const;
    vec4 operator-(const vec4&) const;
    vec4 operator-() const;
    float operator*(const vec4&) const;
    vec4 operator*(float) const;
    vec4 operator/(float) const;

    vec2 xy() const;
    vec3 xyz() const;
    float magnitude() const;
    vec4 normalized() const;

  public:
    float x, y, z, w;
};

class Quaternion {
  public:
    Quaternion() = delete;
    Quaternion(const Quaternion&) = default;
    Quaternion(Quaternion&&) = default;
    Quaternion(float, float, float, float);
    Quaternion(float, const vec3&);
    explicit Quaternion(const vec3&);

    ~Quaternion() = default;

    Quaternion& operator=(const Quaternion&) = default;
    Quaternion& operator=(Quaternion&&) = default;
    Quaternion& operator+=(const Quaternion&);
    Quaternion& operator-=(const Quaternion&);
    Quaternion& operator*=(const Quaternion&);
    Quaternion& operator*=(float);
    Quaternion& operator/=(float);

    std::partial_ordering operator<=>(const Quaternion&) const;
    bool operator==(const Quaternion&) const = default;

    Quaternion operator+(const Quaternion&) const;
    Quaternion operator-(const Quaternion&) const;
    Quaternion operator-() const;
    Quaternion operator*(const Quaternion&) const;
    Quaternion operator*(float) const;
    Quaternion operator/(float) const;

    Quaternion conjugate() const;

  private:
    void normalize();

  public:
    float real = 0.0f;
    vec3 imaginary = vec3(0.0f);
};

template <Layout layout = std430>
class alignas(layout == std430 ? 8 : 16) mat2 {
  public:
    mat2() = delete;
    mat2(const mat2&) = default;
    mat2(mat2&&) = default;
    explicit mat2(float);
    mat2(vec2, vec2);
    explicit mat2(const mat3&);
    explicit mat2(const mat4&);

    ~mat2() = default;

    mat2& operator=(const mat2&) = default;
    mat2& operator=(mat2&&) = default;
    mat2& operator+=(const mat2&);
    mat2& operator-=(const mat2&);
    mat2& operator*=(const mat2&);
    mat2& operator*=(float);
    mat2& operator/=(float);

    vec2& operator[](unsigned int);
    const vec2& operator[](unsigned int) const;

    std::partial_ordering operator<=>(const mat2&) const;
    bool operator==(const mat2&) const = default;

    mat2 operator+(const mat2&) const;
    mat2 operator-(const mat2&) const;
    mat2 operator-() const;
    mat2 operator*(const mat2&) const;
    vec2 operator*(const vec2&) const;
    mat2 operator*(float) const;
    mat2 operator/(float) const;

    static mat2 identity();
    static mat2 rotation(float);
    static mat2 scale(const vec2&);

    mat2 transpose() const;
    float determinant() const;
    std::optional<mat2> inverse() const;

  private:
    vec2 m_rows[2] = { vec2(0.0f), vec2(0.0f) };
};

class alignas(16) mat3 {
  public:
    mat3() = delete;
    mat3(const mat3&) = default;
    mat3(mat3&&) = default;
    explicit mat3(float);
    mat3(vec3, vec3, vec3);

    template <Layout layout>
    explicit mat3(const mat2<layout>&);

    explicit mat3(const mat4&);

    ~mat3() = default;

    mat3& operator=(const mat3&) = default;
    mat3& operator=(mat3&&) = default;
    mat3& operator+=(const mat3&);
    mat3& operator-=(const mat3&);
    mat3& operator*=(const mat3&);
    mat3& operator*=(float);
    mat3& operator/=(float);

    vec3& operator[](unsigned int);
    const vec3& operator[](unsigned int) const;

    std::partial_ordering operator<=>(const mat3&) const;
    bool operator==(const mat3&) const = default;

    mat3 operator+(const mat3&) const;
    mat3 operator-(const mat3&) const;
    mat3 operator-() const;
    mat3 operator*(const mat3&) const;
    vec3 operator*(const vec3&) const;
    mat3 operator*(float) const;
    mat3 operator/(float) const;

    static mat3 identity();
    static mat3 rotation(const vec3&);
    static mat3 scale(const vec3&);

    float determinant() const;
    mat3 transpose() const;
    mat3 cofactor() const;
    mat3 adjugate() const;
    std::optional<mat3> inverse() const;

  private:
    vec3 m_rows[3] = { vec3(0.0f), vec3(0.0f), vec3(0.0f) };
};

class alignas(16) mat4 {
  public:
    mat4() = delete;
    mat4(const mat4&) = default;
    mat4(mat4&&) = default;
    explicit mat4(float);
    mat4(vec4, vec4, vec4, vec4);

    template <Layout layout>
    explicit mat4(const mat2<layout>&);

    explicit mat4(const mat3&);

    ~mat4() = default;

    mat4& operator=(const mat4&) = default;
    mat4& operator=(mat4&&) = default;
    mat4& operator+=(const mat4&);
    mat4& operator-=(const mat4&);
    mat4& operator*=(const mat4&);
    mat4& operator*=(float);
    mat4& operator/=(float);

    vec4& operator[](unsigned int);
    const vec4& operator[](unsigned int) const;

    std::partial_ordering operator<=>(const mat4&) const;
    bool operator==(const mat4&) const = default;

    mat4 operator+(const mat4&) const;
    mat4 operator-(const mat4&) const;
    mat4 operator-() const;
    mat4 operator*(const mat4&) const;
    vec4 operator*(const vec4&) const;
    mat4 operator*(float) const;
    mat4 operator/(float) const;

    static mat4 identity();
    static mat4 rotation(const vec3&);
    static mat4 scale(const vec3&);
    static mat4 translation(const vec3&);
    static mat4 view(const vec3&, const vec3&, const vec3&);
    static mat4 perspective(float, float, float, float);

    float determinant() const;
    mat4 transpose() const;
    mat4 cofactor() const;
    mat4 adjugate() const;
    std::optional<mat4> inverse() const;

  private:
    vec4 m_rows[4] = { vec4(0.0f), vec4(0.0f), vec4(0.0f), vec4(0.0f) };
};

} // namespace ge

ge::vec2 operator*(float, const ge::vec2&);
ge::vec3 operator*(float, const ge::vec3&);
ge::vec4 operator*(float, const ge::vec4&);

template <ge::Layout layout>
ge::mat2<layout> operator*(float, const ge::mat2<layout>&);

ge::mat3 operator*(float, const ge::mat3&);
ge::mat4 operator*(float, const ge::mat4&);