#ifndef __Vec3_h
#define __Vec3_h

#include <cmath>
#include <iostream>

namespace tcii::cg
{ // begin namespace tcii::cg

#define ASSERT_REAL(T, msg) static_assert(std::floating_point<T>, msg)

template <typename real>
struct Vec3
{
  ASSERT_REAL(real, "Vec3: floating-point type expected");

  real x;
  real y;
  real z;

  real length() const;
  Vec3 versor() const;

}; // Vec3

template <typename real>
inline Vec3<real>
operator +(const Vec3<real>& u, const Vec3<real>& v)
{
  return {u.x + v.x, u.y + v.y, u.z + v.z};
}

template <typename real>
inline Vec3<real>
operator -(const Vec3<real>& u, const Vec3<real>& v)
{
  return {u.x - v.x, u.y - v.y, u.z - v.z};
}

template <typename real>
inline Vec3<real>
operator *(real s, const Vec3<real>& v)
{
  return {s * v.x, s * v.y, s * v.z};
}

template <typename real>
inline Vec3<real>
operator *(const Vec3<real>& v, real s)
{
  return s * v;
}

template <typename real>
inline real
Vec3<real>::length() const
{
  return std::sqrt(x * x + y * y + z * z);
}

template <typename real>
inline Vec3<real>
Vec3<real>::versor() const
{
  return ((real)1 / length()) * *this;
}

template <typename T>
std::ostream&
operator <<(std::ostream& os, const Vec3<T>& v)
{
  os << '(' << v.x << ',' << v.y << ',' << v.z << ')';
  return os;
}

using Vec3f = Vec3<float>;

} // end namespace tcii::cg

#endif // __Vec3_h