#include "Rectangle2.h"

void
draw_Rectangle2(const Rectangle2* self)
{
  print(&self->p1, "p1");
  print(&self->p2, "p2");
}

Bounds2
bounds_Rectangle2(const Rectangle2* self)
{
  Bounds2 b = createBounds2();

  inflate(&b, self->p1);
  inflate(&b, self->p2);
  return b;
}

void
move_Rectangle2(Rectangle2* self, const Point2& delta)
{
  move(&self->p1, delta);
  move(&self->p2, delta);
}

extern Shape2::VTable* _vTable_Shape2();

inline Rectangle2::VTable
_makeVTable_Rectangle2()
{
  Rectangle2::VTable vTable = *_vTable_Shape2();

  vTable.typeName = "Rectangle";
  vTable.draw = (Shape2::DrawFunc)draw_Rectangle2;
  vTable.bounds = (Shape2::BoundsFunc)bounds_Rectangle2;
  vTable.move = (Shape2::MoveFunc)move_Rectangle2;
  return vTable;
}

Shape2::VTable*
_vTable_Rectangle2()
{
  static Rectangle2::VTable vTable = _makeVTable_Rectangle2();
  return (Shape2::VTable*)&vTable;
}

void
ctor(Rectangle2* self, const Point2& p1, const Point2& p2)
{
  ctor((Shape2*)self);
  self->shape.vptr = _vTable_Rectangle2();
  self->p1 = p1;
  self->p2 = p2;
}
