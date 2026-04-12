#ifndef __Circle2_h
#define __Circle2_h

#include "Shape2.h"

struct Circle2
{
  using VTable = Shape2::VTable;

  Shape2 shape;
  Point2 center;
  int radius;

}; // Circle2

void ctor(Circle2*, const Point2&, int);

inline Circle2*
createCircle2(const Point2& center, int radius)
{
  Circle2* c = new Circle2;

  ctor(c, center, radius);
  return c;
}

inline void
render(const Circle2* self)
{
  render((const Shape2*)self);
}

inline Bounds2
bounds(const Circle2* self)
{
  return bounds((const Shape2*)self);
}

inline void
move(Circle2* self, const Point2& delta)
{
  move((Shape2*)self, delta);
}

#endif // __Circle2_h
