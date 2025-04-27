#pragma once

#include <cmath>
#include <type_traits>

namespace ge {

template <unsigned int N, typename T>
class mat;

template <unsigned int N, typename T = float>
class vec {
  static_assert(
    N > 1 &&
    N < 5 && (
    std::is_same<T, float>::value         ||
    std::is_same<T, double>::value        ||
    std::is_same<T, unsigned int>::value  ||
    std::is_same<T, int>::value
  ), "groot-engine: vec must be of size 2, 3, or 4 and have type float, double, unsigned int, or int");

  friend class mat<N, T>;

  public:
    vec(const vec&) = default;
    vec(vec&&) = default;
    vec(T);

    template <
      typename ...Tps,
      typename = std::enable_if<sizeof...(Tps) == N
    >>
    vec(Tps... args) : m_data{ static_cast<T>(args)... } {}

    template<
      unsigned int M, typename... Tps,
      typename = std::enable_if<(
        M < N                   &&
        sizeof...(Tps) == N - M
    )>>
    vec(vec<M, T> v, Tps... args) {
      for (unsigned int i = 0; i < M; ++i)
        m_data[i] = v[i];

      T extra[] = { static_cast<T>(args)... };

      for (unsigned int i = M; i < N; ++i)
        m_data[i] = extra[i - M];
    }

    template<typename S, typename = std::enable_if<!std::is_same_v<S, T>>>
    vec(vec<N, S> v) {
      for (unsigned int i = 0; i < N; ++i)
        m_data[i] = static_cast<T>(v[i]);
    }

    ~vec() = default;

    vec& operator = (const vec&) = default;
    vec& operator = (vec&&) = default;
    vec& operator += (const vec&);
    vec& operator -= (const vec&);
    vec& operator *= (T);
    vec& operator /= (T);

    T& operator [] (unsigned int);
    const T& operator [] (unsigned int) const;

    bool operator == (const vec&) const;
    bool operator < (const vec&) const;
    bool operator <= (const vec&) const;
    bool operator > (const vec&) const;
    bool operator >= (const vec&) const;

    vec operator + (const vec&) const;
    vec operator - (const vec&) const;
    vec operator - () const;
    float operator * (const vec&) const;
    vec operator / (T) const;

    static vec zero();

    float magnitude() const;
    vec normalized() const;

    template <typename = std::enable_if<N == 3>>
    vec<3, T> cross(const vec& rhs) const {
      return vec<3, T>(
        m_data[1] * rhs[2] - m_data[2] * rhs[1],
        m_data[2] * rhs[0] - m_data[0] * rhs[2],
        m_data[0] * rhs[1] - m_data[1] * rhs[0]
      );
    }

  private:
    vec() = default;

  private:
    T m_data[N];
};

template <unsigned int N, typename T = float>
class mat {
  static_assert(
    N > 1 &&
    N < 5 && (
    std::is_same_v<T, float>          ||
    std::is_same_v<T, double>         ||
    std::is_same_v<T, unsigned int>   ||
    std::is_same_v<T, int>
  ), "groot-engine: mat<N, T> must have N = 2, 3, or 4 and T = float, double, unsigned int, or int");

  public:
    mat(mat&) = default;
    mat(mat&&) = default;

    template <
      typename ...Rows,
      typename = std::enable_if<
        sizeof...(Rows) == N &&
        std::conjunction_v<std::is_convertible<Rows, vec<N, T>>...>
    >>
    mat(Rows... args) : m_data{ static_cast<vec<N, T>>(args)... } {}

    template <unsigned int M, typename = std::enable_if<M < N>>
    mat(mat<M, T> m) {
      for (unsigned int i = 0; i < M; ++i) {
        for (unsigned int j = 0; j < M; ++j)
          m_data[i][j] = m[i][j];
      }
    }

    template <typename S, typename = std::enable_if<!std::is_same_v<T, S>>>
    mat(mat<N, S> m) {
      for (unsigned int i = 0; i < N; ++i)
        m_data[i] = static_cast<vec<N, T>>(m[i]);
    }

    ~mat() = default;

    mat& operator = (mat&) = default;
    mat& operator = (mat&&) = default;
    mat& operator += (const mat&);
    mat& operator -= (const mat&);
    mat& operator *= (const mat&);
    mat& operator *= (T);
    mat& operator /= (T);

    vec<N, T>& operator [] (unsigned int);
    const vec<N, T>& operator [] (unsigned int) const;

    bool operator == (const mat&) const;
    bool operator < (const mat&) const;
    bool operator <= (const mat&) const;
    bool operator > (const mat&) const;
    bool operator >= (const mat&) const;

    mat operator + (const mat&) const;
    mat operator - (const mat&) const;
    mat operator - () const;
    mat operator * (const mat&) const;
    vec<N, T> operator * (const vec<N, T>&) const;
    mat operator / (T) const;

    static mat identity();

    template <typename = std::enable_if<N == 4>>
    static mat<4, T> rotate(vec<3, T> rotator) {
      mat<4, T> x(
        vec<4, T>(1, 0, 0, 0),
        vec<4, T>(0, cos(rotator[0]), -sin(rotator[0]), 0),
        vec<4, T>(0, sin(rotator[0]), cos(rotator[0]), 0),
        vec<4, T>(0, 0, 0, 1)
      );

      mat<4, T> y(
        vec<4, T>(cos(rotator[1]), 0, -sin(rotator[1]), 0),
        vec<4, T>(0, 1, 0, 0),
        vec<4, T>(sin(rotator[1]), 0, cos(rotator[1]), 0),
        vec<4, T>(0, 0, 0, 1)
      );

      mat<4, T> z(
        vec<4, T>(cos(rotator[2]), -sin(rotator[2]), 0, 0),
        vec<4, T>(sin(rotator[2]), cos(rotator[2]), 0, 0),
        vec<4, T>(0, 0, 1, 0),
        vec<4, T>(0, 0, 0, 1)
      );

      return z * x * y;
    }

    template <typename = std::enable_if<N == 4>>
    static mat<4, T> translate(vec<3, T> translator) {
      return mat<4, T>(
        vec<4, T>(1, 0, 0, translator[0]),
        vec<4, T>(0, 1, 0, translator[1]),
        vec<4, T>(0, 0, 1, translator[2]),
        vec<4, T>(0, 0, 0, 1)
      );
    }

    template <typename = std::enable_if<N == 4>>
    static mat<4, T> scale(vec<3, T> scalar) {
      return mat<4, T>(
        vec<4, T>(scalar[0], 0, 0, 0),
        vec<4, T>(0, scalar[1], 0, 0),
        vec<4, T>(0, 0, scalar[2], 0),
        vec<4, T>(0, 0, 0, 1)
      );
    }

    template <typename = std::enable_if<N == 4>>
    static mat<4, T> view(vec<3, T> eye, vec<3, T> target, vec<3, T> up) {
      vec<3, T> w = (target - eye).normalized();
      vec<3, T> u = w.cross(up).normalized();
      vec<3, T> v = w.cross(u);

      return mat<4, T>(
        vec<4, T>(u, -u * eye),
        vec<4, T>(v, -v * eye),
        vec<4, T>(w, -w * eye),
        vec<4, T>(0, 0, 0, 1)
      );
    }

    template <typename = std::enable_if<N == 4>>
    static mat<4, T> perspective(float fov, float ar, float near, float far) {
      float taninv = 1 / (tan(fov / 2));
      float fmninv = 1 / (far - near);

      return mat<4, T>(
        vec<4, T>(taninv / ar, 0, 0, 0),
        vec<4, T>(0, taninv, 0, 0),
        vec<4, T>(0, 0, far * fmninv, -far * near * fmninv),
        vec<4, T>(0, 0, 1, 0)
      );
    }

    mat transpose() const;

  private:
    mat();

  private:
    vec<N, T> m_data[N];
};

} // namespace ge

template <unsigned int N, typename T>
ge::vec<N, T> operator * (T lhs, const ge::vec<N, T>& rhs) {
  ge::vec<N, T> res = ge::vec<N, T>::zero();

  for (unsigned int i = 0; i < N; ++i)
    res[i] = lhs * rhs[i];

  return res;
}

template <unsigned int N, typename T>
ge::mat<N, T> operator * (T lhs, const ge::mat<N, T>& rhs) {
  ge::mat<N, T> res = ge::mat<N, T>::identity();

  for (unsigned int i = 0; i < N; ++i)
    res[i] = lhs * rhs[i];

  return res;
}