#ifndef __Bounds2_h
#define __Bounds2_h

#include "Point2.h"
#include <climits>

struct Bounds2
{
  Point2 p1;
  Point2 p2;

}; // Bounds2

inline Bounds2
createBounds2()
{
  return Bounds2{{INT_MAX, INT_MAX}, {INT_MIN, INT_MIN}};  
}

inline void
inflate(Bounds2* b, const Point2& p)
{
  if (p.x < b->p1.x)
    b->p1.x = p.x;
  if (p.y < b->p1.y)
    b->p1.y = p.y;
  if (p.x > b->p2.x)
    b->p2.x = p.x;
  if (p.y > b->p2.y)
    b->p2.y = p.y;
}

inline void
inflate(Bounds2* b, const Bounds2& bounds)
{
  inflate(b, bounds.p1);
  inflate(b, bounds.p2);
}

inline void
print(const Bounds2* b, const char* label)
{
  printf("%s min<%d,%d> max<%d,%d>\n",
    label,
    b->p1.x,
    b->p1.y,
    b->p2.x,
    b->p2.y);
}

#endif // __Bounds2_h
