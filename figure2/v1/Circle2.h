#ifndef __Circle2_h
#define __Circle2_h

#include "Shape2.h"

struct Circle2
{
  Shape2 shape;
  Point2 center;
  int radius;

}; // Circle2

inline Circle2*
createCircle2(const Point2& center, int radius)
{
  Circle2* c = new Circle2;

  init((Shape2*)c, Shape2::CIRCLE);
  c->center = center;
  c->radius = radius >= 0 ? radius : 0;;
  return c;
}

void draw(const Circle2*);
Bounds2 bounds(const Circle2*);
void move(Circle2*, const Point2&);

#endif // __Circle2_h
