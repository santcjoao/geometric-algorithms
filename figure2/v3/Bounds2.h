#ifndef __Bounds2_h
#define __Bounds2_h

#include "Point2.h"
#include <climits>

class Bounds2
{
public:
  Bounds2():
    _p1{INT_MAX, INT_MAX},
    _p2{INT_MIN, INT_MIN}
  {
    // do nothing 
  }

  auto& min() const
  {
    return _p1;
  }

  auto& max() const
  {
    return _p2;
  }

  void inflate(const Point2& p)
  {
    if (p.x < _p1.x)
      _p1.x = p.x;
    if (p.y < _p1.y)
      _p1.y = p.y;
    if (p.x > _p2.x)
      _p2.x = p.x;
    if (p.y > _p2.y)
      _p2.y = p.y;
  }

  void inflate(const Bounds2& bounds)
  {
    inflate(bounds._p1);
    inflate(bounds._p2);
  }

  void print(const char* label)
  {
    printf("%s min<%d,%d> max<%d,%d>\n", label, _p1.x, _p1.y, _p2.x, _p2.y);
  }

private:
  Point2 _p1;
  Point2 _p2;

}; // Bounds2

#endif // __Bounds2_h
