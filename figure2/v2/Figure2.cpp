#include "Figure2.h"

Figure2
createFigure2()
{
  Figure2 f;

  f.shapes = createShape2List();
  f.lastSelected = nullptr;
  f.currentColor = Color{0, 0, 0};
  f.lastId = 0;
  return f;
}

Shape2*
add(Figure2* f, Shape2* shape)
{
  if (Figure2* p = shape->parent; p != f)
  {
    if (p)
      remove(&p->shapes, shape);
    add(&f->shapes, shape);
    shape->parent = f;
    shape->id = ++f->lastId;
    shape->color = f->currentColor;
    return shape;
  }
  return nullptr;
}

void
render(const Figure2* f)
{
  Bounds2 b = bounds(f);

  puts("**Begin figure");
  print(&b, "bounds");
  for (const Shape2* s = f->shapes.head; s; s = s->next)
    render(s);
  puts("**End figure\n");
}

Bounds2
bounds(const Figure2* f)
{
  Bounds2 b = createBounds2();

  for (const Shape2* s = f->shapes.head; s; s = s->next)
    inflate(&b, bounds(s));
  return b;
}

void
move(Figure2* f, const Point2& delta)
{
  for (Shape2* s = f->lastSelected; s; s = s->nextSelected)
    move(s, delta);
}

void
setColor(Figure2* f, const Color& color)
{
  for (Shape2* s = f->lastSelected; s; s = s->nextSelected)
    s->color = color;
}

void
clearSelection(Figure2* f)
{
  while (f->lastSelected)
  {
    Shape2* temp = f->lastSelected;

    f->lastSelected = temp->nextSelected;
    remove(&f->shapes, temp);
    delete temp;
  }
}

Shape2*
select(Figure2* f, int id)
{
  for (Shape2* s = f->shapes.head; s; s = s->next)
    if (s->id == id)
    {
      if (!s->selected)
      {
        s->selected = true;
        s->nextSelected = f->lastSelected;
        f->lastSelected = s;
      }
      return s;
    }
  return nullptr;
}

void
selectAll(Figure2* f)
{
  f->lastSelected = nullptr;
  for (Shape2* s = f->shapes.head; s; s = s->next)
    {
      s->selected = true;
      s->nextSelected = f->lastSelected;
      f->lastSelected = s;
    }   
}

void
deselectAll(Figure2* f)
{
  while (f->lastSelected)
  {
    Shape2* temp = f->lastSelected;

    f->lastSelected = temp->nextSelected;
    temp->nextSelected = nullptr;
    temp->selected = false;
  }
}
