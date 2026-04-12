#include "Rectangle2.h"

void
Rectangle2::draw() const
{
  p1.print("p1");
  p2.print("p2");
}

Bounds2
Rectangle2::bounds() const
{
  Bounds2 b;

  b.inflate(p1);
  b.inflate(p2);
  return b;
}

void
Rectangle2::move(const Point2& delta)
{
  p1.move(delta);
  p2.move(delta);
}
