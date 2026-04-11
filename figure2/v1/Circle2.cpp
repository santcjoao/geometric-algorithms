#include "Circle2.h"

void
draw(const Circle2* c)
{
  print(&c->center, "center");
  printf("radius %d\n", c->radius);
}

Bounds2
bounds(const Circle2* c)
{
  return Bounds2
  {
    {c->center.x - c->radius, c->center.y - c->radius},
    {c->center.x + c->radius, c->center.y + c->radius}
  };
}

void
move(Circle2* c, const Point2& delta)
{
  move(&c->center, delta);
}
