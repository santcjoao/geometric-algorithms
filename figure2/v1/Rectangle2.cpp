#include "Rectangle2.h"

void
draw(const Rectangle2* r)
{
  print(&r->p1, "p1");
  print(&r->p2, "p2");
}

Bounds2
bounds(const Rectangle2* r)
{
  Bounds2 b = createBounds2();

  inflate(&b, r->p1);
  inflate(&b, r->p2);
  return b;
}

void
move(Rectangle2* r, const Point2& delta)
{
  move(&r->p1, delta);
  move(&r->p2, delta);
}
