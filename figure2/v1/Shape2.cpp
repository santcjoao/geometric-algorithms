#include "Circle2.h"
#include "Line2.h"
#include "Rectangle2.h"

inline void
beginRender(const Shape2* s)
{
  static const char* typeName[]
  {
    "Line2",
    "Rectangle2",
    "Circle2"
  };

  printf("**%s %d%s\n",
    typeName[s->type],
    s->id,
    s->selected ? " (selected)" : "");
}

void
endRender(const Shape2* s)
{
  Bounds2 b = bounds(s);

  print(&b, "bounds");
  print(&s->color, "color");
}

void
render(const Shape2* s)
{
  beginRender(s);
  switch (s->type)
  {
    case Shape2::LINE:
      draw((const Line2*)s);
      break;
    case Shape2::RECTANGLE:
      draw((const Rectangle2*)s);
      break;
    case Shape2::CIRCLE:
      draw((const Circle2*)s);
      break;
  }
  endRender(s);
}

Bounds2
bounds(const Shape2* s)
{
  Bounds2 b;

  switch (s->type)
  {
    case Shape2::LINE:
      b = bounds((const Line2*)s);
      break;
    case Shape2::RECTANGLE:
      b = bounds((const Rectangle2*)s);
      break;
    case Shape2::CIRCLE:
      b = bounds((const Circle2*)s);
      break;
  }
  return b;
}

void
move(Shape2* s, const Point2& delta)
{
  switch (s->type)
  {
    case Shape2::LINE:
      move((Line2*)s, delta);
      break;
    case Shape2::RECTANGLE:
      move((Rectangle2*)s, delta);
      break;
    case Shape2::CIRCLE:
      move((Circle2*)s, delta);
      break;
  }
}
