#ifndef __Line2_h
#define __Line2_h

#include "Shape2.h"

class Line2: public Shape2
{
public:
  Point2 p1;
  Point2 p2;

  Line2(const Point2& p1, const Point2& p2):
    p1{p1},
    p2{p2}
  {
    // do nothing
  }

  Bounds2 bounds() const override;
  void move(const Point2&) override;

protected:
  void draw() const override;

}; // Line2

#endif // __Line2_h
