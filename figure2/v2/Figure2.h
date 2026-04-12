#ifndef __Figure2_h
#define __Figure2_h

#include "Circle2.h"
#include "Line2.h"
#include "Rectangle2.h"
#include "Shape2List.h"

struct Figure2
{
  Shape2List shapes;
  Shape2* lastSelected;
  Color currentColor;
  int lastId;

}; // Figure2

Figure2 createFigure2();

inline void
destroyFigure2(Figure2* f)
{
  destroyShape2List(&f->shapes);
}

inline void
clear(Figure2* f)
{
  clear(&f->shapes);
  f->lastSelected = nullptr;
  f->lastId = 0;
}

Shape2* add(Figure2*, Shape2*);

inline Shape2*
add(Figure2* f, Line2* line)
{
  return add(f, (Shape2*)line);
}

inline Shape2*
addLine(Figure2* f, const Point2& p1, const Point2& p2)
{
  return add(f, createLine2(p1, p2));
}

inline Shape2*
add(Figure2* f, Rectangle2* rect)
{
  return add(f, (Shape2*)rect);
}

inline Shape2*
addRectangle(Figure2* f, const Point2& p1, const Point2& p2)
{
  return add(f, createRectangle2(p1, p2));
}

inline Shape2*
add(Figure2* f, Circle2* circle)
{
  return add(f, (Shape2*)circle);
}

inline Shape2*
addCircle(Figure2* f, const Point2& center, int radius)
{
  return add(f, createCircle2(center, radius));
}

void render(const Figure2*);
Bounds2 bounds(const Figure2*);
void move(Figure2*, const Point2&);
void setColor(Figure2*, const Color&);
void clearSelection(Figure2*);
Shape2* select(Figure2*, int);
void selectAll(Figure2*);
void deselectAll(Figure2*);

#endif // __Figure2_h
