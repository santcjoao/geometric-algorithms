#ifndef __Point2_h
#define __Point2_h

#include <cstdio>

struct Point2
{
  int x;
  int y;

}; // Point2

inline void
move(Point2* p, const Point2& delta)
{
  p->x += delta.x;
  p->y += delta.y;
}

inline void
print(const Point2* p, const char* label)
{
  printf("%s<%d,%d>\n", label, p->x, p->y);
}

#endif // __Point2_h
