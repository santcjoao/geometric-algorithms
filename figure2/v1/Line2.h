#ifndef __Line2_h
#define __Line2_h

#include "Shape2.h"

struct Line2
{
  Shape2 shape;
  Point2 p1;
  Point2 p2;

}; // Line2

inline Line2*
createLine2(const Point2& p1, const Point2& p2)
{
  Line2* l = new Line2;

  init((Shape2*)l, Shape2::LINE);
  l->p1 = p1;
  l->p2 = p2;
  return l;
}

void draw(const Line2*);
Bounds2 bounds(const Line2*);
void move(Line2*, const Point2&);

#endif // __Line2_h
