#ifndef __Rectangle2_h
#define __Rectangle2_h

#include "Shape2.h"

struct Rectangle2
{
  using VTable = Shape2::VTable;

  Shape2 shape;
  Point2 p1;
  Point2 p2;

}; // Rectangle2

void ctor(Rectangle2*, const Point2&, const Point2&);

inline Rectangle2*
createRectangle2(const Point2& p1, const Point2& p2)
{
  Rectangle2* r = new Rectangle2;

  ctor(r, p1, p2);
  return r;
}

inline void
render(const Rectangle2* self)
{
  render((const Shape2*)self);
}

inline Bounds2
bounds(const Rectangle2* self)
{
  return bounds((const Shape2*)self);
}

inline void
move(Rectangle2* self, const Point2& delta)
{
  move((Shape2*)self, delta);
}

#endif // __Rectangle2_h
