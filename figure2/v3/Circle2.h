#ifndef __Circle2_h
#define __Circle2_h

#include "Shape2.h"

class Circle2: public Shape2
{
public:
  Point2 center;

  Circle2(const Point2& center, int radius):
    center{center}
  {
    setRadius(radius);
  }

  auto radius() const
  {
    return _radius;
  }

  void setRadius(int radius)
  {
    _radius = radius >= 0 ? radius : 0;
  }

  Bounds2 bounds() const override;
  void move(const Point2&) override;

protected:
  void draw() const override;

private:
  int _radius;

}; // Circle2

#endif // __Circle2_h
