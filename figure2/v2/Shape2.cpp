#include "Shape2.h"

void
render_Shape2(const Shape2* self)
{
  self->vptr->beginRender(self);
  self->vptr->draw(self);
  self->vptr->endRender(self);
}

void
beginRender_Shape2(const Shape2* self)
{
  printf("**%s %d%s\n",
    self->vptr->typeName,
    self->id,
    self->selected ? " (selected)" : "");
}

void
endRender_Shape2(const Shape2* self)
{
  Bounds2 b = self->vptr->bounds(self);

  print(&b, "bounds");
  print(&self->color, "color");
}

inline Shape2::VTable
_makeVTable_Shape2()
{
  return Shape2::VTable
  {
    .typeName = "Shape2",
    .render = render_Shape2,
    .beginRender = beginRender_Shape2,
    .endRender = endRender_Shape2
  };
}

Shape2::VTable*
_vTable_Shape2()
{
  static Shape2::VTable vTable = _makeVTable_Shape2();
  return &vTable;
}

void
ctor(Shape2* self)
{
  self->vptr = _vTable_Shape2();
  self->parent = nullptr;
  self->id = 0;
}
