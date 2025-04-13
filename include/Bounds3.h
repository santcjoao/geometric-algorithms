#ifndef __Bounds3_h
#define __Bounds3_h

#include "Vec3.h"

namespace tcii::cg
{ // begin namespace tcii::cg

template <typename real>
class Bounds3
{
public:
  using vec3 = Vec3<real>;

  Bounds3()
  {
    setEmpty();
  }

  auto& min() const
  {
    return _p1;
  }

  auto& max() const
  {
    return _p2;
  }

  bool contains(const vec3& p) const
  {
    if (p.x < _p1.x || p.x > _p2.x)
      return false;
    if (p.y < _p1.y || p.y > _p2.y)
      return false;
    if (p.z < _p1.z || p.z > _p2.z)
      return false;
    return true;
  }

  void setEmpty()
  {
    _p1.x = _p1.y = _p1.z = +std::numeric_limits<real>::max();
    _p2.x = _p2.y = _p2.z = -std::numeric_limits<real>::max();
  }

  void inflate(const vec3& p)
  {
    if (p.x < _p1.x)
      _p1.x = p.x;
    if (p.x > _p2.x)
      _p2.x = p.x;
    if (p.y < _p1.y)
      _p1.y = p.y;
    if (p.y > _p2.y)
      _p2.y = p.y;
    if (p.z < _p1.z)
      _p1.z = p.z;
    if (p.z > _p2.z)
      _p2.z = p.z;
  }

  void inflate(const Bounds3& bounds)
  {
    inflate(bounds._p1);
    inflate(bounds._p2);
  }

private:
  vec3 _p1;
  vec3 _p2;

}; // Bounds3

template <typename real>
std::ostream&
operator <<(std::ostream& os, const Bounds3<real>& b)
{
  os << "min" << b.min() << " max" << b.max();
  return os;
}

using Bounds3f = Bounds3<float>;

} // end namespace tcii::cg

#endif // __Bounds3_h