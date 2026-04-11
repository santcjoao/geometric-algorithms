#ifndef __Color_h
#define __Color_h

#include <cstdio>

using byte = unsigned char;

struct Color
{
  byte r;
  byte g;
  byte b;

}; // Color

inline void
print(const Color* c, const char* label)
{
  printf("%s(%d,%d,%d)\n", label, c->r, c->g, c->b);
}

#endif // __Color_h
