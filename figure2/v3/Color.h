#ifndef __Color_h
#define __Color_h

#include <cstdio>

using byte = unsigned char;

struct Color
{
  byte r;
  byte g;
  byte b;

  void print(const char* label) const
  {
    printf("%s(%d,%d,%d)\n", label, r, g, b);
  }

  static const Color black;
  static const Color red;
  static const Color green;
  static const Color blue;
  static const Color white;

}; // Color

inline constexpr Color Color::black{};
inline constexpr Color Color::red{.r = 255};
inline constexpr Color Color::green{.g = 255};
inline constexpr Color Color::blue{.b = 255};
inline constexpr Color Color::white{255, 255, 255};

#endif // __Color_h
