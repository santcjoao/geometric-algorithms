#include "Circle2.h"

void
Circle2::draw() const
{
  center.print("center");
  printf("radius %d\n", _radius);
}

Bounds2
Circle2::bounds() const
{
  Bounds2 b;

  b.inflate({center.x - _radius, center.y - _radius});
  b.inflate({center.x + _radius, center.y + _radius});
  return b;
}

void
Circle2::move(const Point2& delta)
{
  center.move(delta);
}
