#ifndef __Rectangle2_h
#define __Rectangle2_h

#include "Shape2.h"

struct Rectangle2
{
  Shape2 shape;
  Point2 p1;
  Point2 p2;

}; // Rectangle2

inline Rectangle2*
createRectangle2(const Point2& p1, const Point2& p2)
{
  Rectangle2* r = new Rectangle2;

  init((Shape2*)r, Shape2::RECTANGLE);
  r->p1 = p1;
  r->p2 = p2;
  return r;
}

void draw(const Rectangle2*);
Bounds2 bounds(const Rectangle2*);
void move(Rectangle2*, const Point2&);

#endif // __Rectangle2_h
