#ifndef __Shape2_h
#define __Shape2_h

#include "Bounds2.h"
#include "Color.h"

struct Figure2;

struct Shape2
{
  using RenderFunc = void (*)(const Shape2*);
  using BeginRenderFunc = void (*)(const Shape2*);
  using EndRenderFunc = void (*)(const Shape2*);
  using DrawFunc = void (*)(const Shape2*);
  using BoundsFunc = Bounds2 (*)(const Shape2*);
  using MoveFunc = void (*)(Shape2*, const Point2&);

  struct VTable
  {
    const char* typeName;
    RenderFunc render;
    BeginRenderFunc beginRender;
    EndRenderFunc endRender;
    DrawFunc draw;
    BoundsFunc bounds;
    MoveFunc move;

  }; // VTable

  VTable* vptr;
  Figure2* parent;
  int id;
  Color color;
  Shape2* next;
  Shape2* previous;
  Shape2* nextSelected;
  bool selected;

}; // Shape2

void ctor(Shape2*);

inline void
render(const Shape2* self)
{
  self->vptr->render(self);
}

inline Bounds2
bounds(const Shape2* self)
{
  return self->vptr->bounds(self);
}

inline void
move(Shape2* self, const Point2& delta)
{
  self->vptr->move(self, delta);
}

#endif // __Shape2_h
