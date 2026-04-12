#ifndef __Line2_h
#define __Line2_h

#include "Shape2.h"

struct Line2
{
  using VTable = Shape2::VTable;

  Shape2 shape;
  Point2 p1;
  Point2 p2;

}; // Line2

void ctor(Line2*, const Point2&, const Point2&);

inline Line2*
createLine2(const Point2& p1, const Point2& p2)
{
  Line2* l = new Line2;

  ctor(l, p1, p2);
  return l;
}

inline void
render(const Line2* self)
{
  render((const Shape2*)self);
}

inline Bounds2
bounds(const Line2* self)
{
  return bounds((const Shape2*)self);
}

inline void
move(Line2* self, const Point2& delta)
{
  move((Shape2*)self, delta);
}

#endif // __Line2_h
