#include "Line2.h"

void
draw(const Line2* l)
{
  print(&l->p1, "p1");
  print(&l->p2, "p2");
}

Bounds2
bounds(const Line2* l)
{
  Bounds2 b = createBounds2();

  inflate(&b, l->p1);
  inflate(&b, l->p2);
  return b;
}

void
move(Line2* l, const Point2& delta)
{
  move(&l->p1, delta);
  move(&l->p2, delta);
}
