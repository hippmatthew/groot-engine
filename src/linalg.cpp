#include "src/include/linalg.hpp"

#include <cmath>
#include <stdexcept>

namespace ge {

template <unsigned int N, typename T>
vec<N, T>::vec(T val) {
  for (unsigned int i = 0; i < N; ++i)
    m_data[i] = val;
}

template <unsigned int N, typename T>
vec<N, T>& vec<N, T>::operator += (const vec& rhs) {
  if (&rhs == this) return *this;

  for (unsigned int i = 0; i < N; ++i)
    m_data[i] += rhs[i];

  return *this;
}

template <unsigned int N, typename T>
vec<N, T>& vec<N, T>::operator -= (const vec& rhs) {
  if (&rhs == this) return *this;

  for (unsigned int i = 0; i < N; ++i)
    m_data[i] -= rhs[i];

  return *this;
}

template <unsigned int N, typename T>
vec<N, T>& vec<N, T>::operator *= (T rhs) {
  for (unsigned int i = 0; i < N; ++i)
    m_data[i] *= rhs;

  return *this;
}

template <unsigned int N, typename T>
vec<N, T>& vec<N, T>::operator /= (T rhs) {
  for (unsigned int i = 0; i < N; ++i)
    m_data[i] /= rhs;

  return *this;
}

template <unsigned int N, typename T>
T& vec<N, T>::operator [] (unsigned int index) {
  if (index > N - 1)
    throw std::out_of_range("groot-engine: vec index out of range");

  return m_data[index];
}

template <unsigned int N, typename T>
const T& vec<N, T>::operator [] (unsigned int index) const {
  if (index > N - 1)
    throw std::out_of_range("groot-engine: vec index out of range");

  return m_data[index];
}

template <unsigned int N, typename T>
bool vec<N, T>::operator == (const vec& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] != rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
bool vec<N, T>::operator < (const vec& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] >= rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
bool vec<N, T>::operator <= (const vec& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] > rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
bool vec<N, T>::operator > (const vec& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] <= rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
bool vec<N, T>::operator >= (const vec& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] < rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
vec<N, T> vec<N, T>::operator + (const vec& rhs) const {
  vec res;

  for (unsigned int i = 0; i < N; ++i)
    res[i] = m_data[i] + rhs[i];

  return res;
}

template <unsigned int N, typename T>
vec<N, T> vec<N, T>::operator - (const vec& rhs) const {
  return *this + (-rhs);
}

template <unsigned int N, typename T>
vec<N, T> vec<N, T>::operator - () const {
  return static_cast<T>(-1) * *this;
}

template <unsigned int N, typename T>
float vec<N, T>::operator * (const vec& rhs) const {
  float sum = 0;

  for (unsigned int i = 0; i < N; ++i)
    sum += m_data[i] * rhs[i];

  return sum;
}

template <unsigned int N, typename T>
vec<N, T> vec<N, T>::operator / (T rhs) const {
  return (1 / rhs) * *this;
}

template <unsigned int N, typename T>
vec<N, T> vec<N, T>::zero() {
  return vec();
}

template <unsigned int N, typename T>
float vec<N, T>::magnitude() const {
  return std::sqrt(*this * *this);
}

template <unsigned int N, typename T>
vec<N, T> vec<N, T>::normalized() const {
  return *this / magnitude();
}

template <unsigned int N, typename T>
mat<N, T>& mat<N, T>::operator += (const mat& rhs) {
  for (unsigned int i = 0; i < N; ++i)
    m_data[i] += rhs[i];

  return *this;
}

template <unsigned int N, typename T>
mat<N, T>& mat<N, T>::operator -= (const mat& rhs) {
  for (unsigned int i = 0; i < N; ++i)
    m_data[i] -= rhs[i];

  return *this;
}

template <unsigned int N, typename T>
mat<N, T>& mat<N, T>::operator *= (const mat& rhs) {
  mat t = rhs.transpose();

  for (unsigned int i = 0; i < N; ++i) {
    vec<N, T> row;

    for (unsigned int j = 0; j < N; ++j)
      row[j] = m_data[i] * t[j];

    m_data[i] = row;
  }

  return *this;
}

template <unsigned int N, typename T>
mat<N, T>& mat<N, T>::operator *= (T rhs) {
  for (unsigned int i = 0; i < N; ++i)
    m_data[i] *= rhs;

  return *this;
}

template <unsigned int N, typename T>
mat<N, T>& mat<N, T>::operator /= (T rhs) {
  for (unsigned int i = 0; i < N; ++i)
    m_data[i] /= rhs;

  return *this;
}

template <unsigned int N, typename T>
vec<N, T>& mat<N, T>::operator [] (unsigned int index) {
  if (index > N - 1)
    throw std::out_of_range("groot-engine: mat index out of range");

  return m_data[index];
}

template <unsigned int N, typename T>
const vec<N, T>& mat<N, T>::operator [] (unsigned int index) const {
  if (index > N - 1)
    throw std::out_of_range("groot-engine: mat index out of range");

  return m_data[index];
}

template <unsigned int N, typename T>
bool mat<N, T>::operator == (const mat& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] != rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
bool mat<N, T>::operator < (const mat& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] >= rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
bool mat<N, T>::operator <= (const mat& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] > rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
bool mat<N, T>::operator > (const mat& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] <= rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
bool mat<N, T>::operator >= (const mat& rhs) const {
  for (unsigned int i = 0; i < N; ++i)
    if (m_data[i] < rhs[i]) return false;
  return true;
}

template <unsigned int N, typename T>
mat<N, T> mat<N, T>::operator + (const mat& rhs) const {
  mat m;

  for (unsigned int i = 0; i < N; ++i)
    m[i] = m_data[i] + rhs[i];

  return m;
}

template <unsigned int N, typename T>
mat<N, T> mat<N, T>::operator - (const mat& rhs) const {
  mat m;

  for (unsigned int i = 0; i < N; ++i)
    m[i] = m_data[i] - rhs[i];

  return m;
}

template <unsigned int N, typename T>
mat<N, T> mat<N, T>::operator - () const {
  mat m;

  for (unsigned int i = 0; i < N; ++i)
    m[i] = -m_data[i];

  return m;
}

template <unsigned int N, typename T>
mat<N, T> mat<N, T>::operator * (const mat& rhs) const {
  mat m;
  mat t = rhs.transpose();

  for (unsigned int i = 0; i < N; ++i) {
    vec<N, T> row;
    for (unsigned int j = 0; j < N; ++j)
      m[i][j] = m_data[i] * t[j];
  }

  return m;
}

template <unsigned int N, typename T>
vec<N, T> mat<N, T>::operator * (const vec<N, T>& rhs) const {
  vec<N, T> v;

  for (unsigned int i = 0; i < N; ++i)
    v[i] = m_data[i] * rhs;

  return v;
}

template <unsigned int N, typename T>
mat<N, T> mat<N, T>::operator / (T rhs) const {
  mat m;

  for (unsigned int i = 0; i < N; ++i)
    m[i] = m_data[i] / rhs;

  return m;
}

template <unsigned int N, typename T>
mat<N, T> mat<N, T>::identity() {
  return mat();
}

template <unsigned int N, typename T>
mat<N, T> mat<N, T>::transpose() const {
  mat t;

  for (unsigned int i = 0; i < N; ++i) {
    for (unsigned int j = 0; j < N; ++j)
      t[i][j] = m_data[j][i];
  }

  return t;
}

template <unsigned int N, typename T>
mat<N, T>::mat() {
  for (unsigned int i = 0; i < N; ++i)
    m_data[i][i] = 1;
}

template class vec<2, float>;
template class vec<3, float>;
template class vec<4, float>;
template class vec<2, double>;
template class vec<3, double>;
template class vec<4, double>;
template class vec<2, unsigned int>;
template class vec<3, unsigned int>;
template class vec<4, unsigned int>;
template class vec<2, int>;
template class vec<3, int>;
template class vec<4, int>;

template class mat<2, float>;
template class mat<3, float>;
template class mat<4, float>;
template class mat<2, double>;
template class mat<3, double>;
template class mat<4, double>;
template class mat<2, unsigned int>;
template class mat<3, unsigned int>;
template class mat<4, unsigned int>;
template class mat<2, int>;
template class mat<3, int>;
template class mat<4, int>;

} // namespace ge