#include "Circle2.h"

void
draw_Circle2(const Circle2* c)
{
  print(&c->center, "center");
  printf("radius %d\n", c->radius);
}

Bounds2
bounds_Circle2(const Circle2* c)
{
  return Bounds2
  {
    {c->center.x - c->radius, c->center.y - c->radius},
    {c->center.x + c->radius, c->center.y + c->radius}
  };
}

void
move_Circle2(Circle2* c, const Point2& delta)
{
  move(&c->center, delta);
}

extern Shape2::VTable* _vTable_Shape2();

inline Circle2::VTable
_makeVTable_Circle2()
{
  Circle2::VTable vTable = *_vTable_Shape2();

  vTable.typeName = "Circle2";
  vTable.draw = (Shape2::DrawFunc)draw_Circle2;
  vTable.bounds = (Shape2::BoundsFunc)bounds_Circle2;
  vTable.move = (Shape2::MoveFunc)move_Circle2;
  return vTable;
}

Shape2::VTable*
_vTable_Circle2()
{
  static Circle2::VTable vTable = _makeVTable_Circle2();
  return (Shape2::VTable*)&vTable;
}

void
ctor(Circle2* self, const Point2& center, int radius)
{
  ctor((Shape2*)self);
  self->shape.vptr = _vTable_Circle2();
  self->center = center;
  self->radius = radius >= 0 ? radius : 0;
}
