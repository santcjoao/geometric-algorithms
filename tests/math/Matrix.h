#ifndef __Matrix_h
#define __Matrix_h

#include "Vector.h"
#include <cstdio>

// An *academic* example of generic dynamic matrix by Paulo Pagliosa

/*
The template class Matrix below *is not* designed for efficiency and
its use in a real-world program is not encouraged. The purpose of
this example is just to demonstrate some C++ features such as
exception throwing, operator overloading, and copy/move operations.
*/

namespace tcii::tests::math
{ // begin namespace tcii::tests::math

enum class MatrixIndexType
{
  Row,
  Col
};

inline auto
indexOutOfRange(MatrixIndexType it, size_t i, size_t n)
{
  cg::error<std::logic_error>("Matrix index out of range: "
    "%s %llud not in [0,%llu)",
    it == MatrixIndexType::Row ? "row" : "column",
    i,
    n);
}

inline auto
dimensionsMustAgree(size_t m1, size_t n1, size_t m2, size_t n2)
{
  cg::error<std::logic_error>("Matrix dimensions must agree: "
    "(%llu,%llu), (%llu,%llu)",
    m1, n1,
    m2, n2);
}

template <typename T>
class Matrix
{
public:
  using value_type = T;

  struct Element
  {
    int i;
    int j;
    T value;
  };

  using IterFunc = void(const Element&);

  void iterate(IterFunc) const;

  Matrix() = default;

  Matrix(size_t m, size_t n)
  {
    _m = m;
    _n = n;
    _data = new T[m * n];
  }

  Matrix(const Matrix&);
  Matrix(Matrix&&) noexcept;

  ~Matrix()
  {
    delete []_data;
  }

  auto rows() const
  {
    return _m;
  }

  auto cols() const
  {
    return _n;
  }

  auto& operator ()(size_t i, size_t j)
  {
#ifdef DEBUG
    if (i >= _m)
      indexOutOfRange(MatrixIndexType::Row, i, _m);
    if (j >= _n)
      indexOutOfRange(MatrixIndexType::Col, j, _n);
#endif // DEBUG
    return _data[i * _n + j];
  }

  auto operator ()(size_t i, size_t j) const
  {
    return const_cast<Matrix*>(this)->operator ()(i, j);
  }

  Matrix& operator =(const Matrix&);
  Matrix& operator =(Matrix&&) noexcept;

  Matrix& operator +=(const Matrix&);
  Matrix& operator -=(const Matrix&);
  Matrix& operator *=(const Matrix&);
  Matrix& operator *=(const T&);

  Matrix operator +(const Matrix&) const;
  Matrix operator -(const Matrix&) const;
  Matrix operator *(const Matrix&) const;
  Vector<T> operator *(const Vector<T>&) const;
  Matrix operator *(const T&) const;

  Matrix transpose() const;

private:
  size_t _m{};
  size_t _n{};
  T* _data{};

}; // Matrix

template <typename T>
Matrix<T>::Matrix(const Matrix& other):
  Matrix{other._m, other._n}
{
#ifdef DEBUG
  puts("**Matrix copy ctor**");
#endif // DEBUG
  for (auto s = _m * _n, i = 0; i < s; ++i)
    _data[i] = other._data[i];
}

template <typename T>
Matrix<T>::Matrix(Matrix&& other) noexcept
{
#ifdef DEBUG
  puts("**Matrix move ctor**");
#endif // DEBUG
  _m = other._m;
  _n = other._n;
  _data = other._data;
  other._m = other._n = 0;
  other._data = nullptr;
}

template <typename T>
Matrix<T>&
Matrix<T>::operator =(const Matrix& other)
{
#ifdef DEBUG
  puts("**Matrix copy op**");
#endif // DEBUG
  if (_m != other._m || _n != other._n)
  {
    delete []_data;
    _m = other._m;
    _n = other._n;
    _data = new float[_m * _n];
  }
  for (size_t s = _m * _n, i{}; i < s; ++i)
    _data[i] = other._data[i];
  return *this;
}

template <typename T>
Matrix<T>&
Matrix<T>::operator =(Matrix&& other) noexcept
{
#ifdef DEBUG
  puts("**Matrix move op**");
#endif // DEBUG
  delete []_data;
  _m = other._m;
  _n = other._n;
  _data = other._data;
  other._m = other._n = 0;
  other._data = nullptr;
  return *this;
}

template <typename T>
Matrix<T>
Matrix<T>::operator +(const Matrix& b) const
{
#ifdef DEBUG
  if (_m != b._m || _n != b._n)
    dimensionsMustAgree(_m, _n, b._m, b._n);
#endif // DEBUG

  Matrix c{_m, _n};

  for (size_t s = _m * _n, i{}; i < s; ++i)
    c._data[i] = _data[i] + b._data[i];
  return c;
}

template <typename T>
Matrix<T>&
Matrix<T>::operator +=(const Matrix& b)
{
#ifdef DEBUG
  if (_m != b._m || _n != b._n)
    dimensionsMustAgree(_m, _n, b._m, b._n);
#endif // DEBUG
  for (size_t s = _m * _n, i{}; i < s; ++i)
    _data[i] += b._data[i];
  return *this;
}

template <typename T>
Vector<T>
Matrix<T>::operator *(const Vector<T>& b) const
{
#ifdef DEBUG
  if (_n != b.size())
    dimensionsMustAgree(_n, b.size());
#endif // DEBUG

  Vector<T> c{_m};

  for (size_t p{}, i{}; i < _m; ++i)
  {
    T s{};

    for (size_t j{}; j < _n; ++j)
      s += _data[p++] * b(j);
    c(i) = s;
  }
  return c;
}

// **Exercise: implement here the other matrix ops

template <typename T>
void
Matrix<T>::iterate(IterFunc f) const
{
  Element e;

  e.i = 0;
  for (int p = 0; e.i < _m; ++e.i)
    for (e.j = 0; e.j < _n; ++e.j)
    {
      e.value = _data[p++];
      f(e);
    }
}

} // end namespace tcii::tests::math

template <typename T>
inline auto
operator *(const T& s, const tcii::tests::math::Matrix<T>& m)
{
  return m * s;
}

#endif // __Matrix_h
