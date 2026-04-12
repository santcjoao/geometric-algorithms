#include "Line2.h"

void
draw_Line2(const Line2* self)
{
  print(&self->p1, "p1");
  print(&self->p2, "p2");
}

Bounds2
bounds_Line2(const Line2* self)
{
  Bounds2 b = createBounds2();

  inflate(&b, self->p1);
  inflate(&b, self->p2);
  return b;
}

void
move_Line2(Line2* self, const Point2& delta)
{
  move(&self->p1, delta);
  move(&self->p2, delta);
}

extern Shape2::VTable* _vTable_Shape2();

inline Line2::VTable
_makeVTable_Line2()
{
  Line2::VTable vTable = *_vTable_Shape2();

  vTable.typeName = "Line2";
  vTable.draw = (Shape2::DrawFunc)draw_Line2;
  vTable.bounds = (Shape2::BoundsFunc)bounds_Line2;
  vTable.move = (Shape2::MoveFunc)move_Line2;
  return vTable;
}

Shape2::VTable*
_vTable_Line2()
{
  static Line2::VTable vTable = _makeVTable_Line2();
  return (Shape2::VTable*)&vTable;
}

void
ctor(Line2* self, const Point2& p1, const Point2& p2)
{
  ctor((Shape2*)self);
  self->shape.vptr = _vTable_Line2();
  self->p1 = p1;
  self->p2 = p2;
}
