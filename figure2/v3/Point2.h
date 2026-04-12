#ifndef __Point2_h
#define __Point2_h

#include <cstdio>

struct Point2
{
  int x;
  int y;

  void move(const Point2& delta)
  {
    x += delta.x;
    y += delta.y;
  }

  void print(const char* label) const
  {
    printf("%s<%d,%d>\n", label, x, y);
  }

}; // Point2

#endif // __Point2_h
