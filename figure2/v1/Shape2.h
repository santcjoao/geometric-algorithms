#ifndef __Shape2_h
#define __Shape2_h

#include "Bounds2.h"
#include "Color.h"

struct Figure2;

struct Shape2
{
  enum Type
  {
    LINE,
    RECTANGLE,
    CIRCLE
  };

  Type type;
  Figure2* parent;
  int id;
  Color color;
  Shape2* next;
  Shape2* previous;
  Shape2* nextSelected;
  bool selected;

}; // Shape2

inline void
init(Shape2* shape, Shape2::Type type)
{
  shape->type = type;
  shape->parent = nullptr;
  shape->id = 0;
}

void render(const Shape2*);
Bounds2 bounds(const Shape2*);
void move(Shape2*, const Point2&);

#endif // __Shape2_h
