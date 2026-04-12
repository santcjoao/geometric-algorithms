#include "Line2.h"

void
Line2::draw() const
{
  p1.print("p1");
  p2.print("p2");
}

Bounds2
Line2::bounds() const
{
  Bounds2 b;

  b.inflate(p1);
  b.inflate(p2);
  return b;
}

void
Line2::move(const Point2& delta)
{
  p1.move(delta);
  p2.move(delta);
}
