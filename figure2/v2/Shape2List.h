#ifndef __Shape2List_h
#define __Shape2List_h

#include "Shape2.h"
struct Shape2List
{
  Shape2* head;
  Shape2* tail;
  int count;

}; // Shape2List

inline Shape2List
createShape2List()
{
  return Shape2List{nullptr, nullptr, 0};
}

void add(Shape2List*, Shape2*);
void remove(Shape2List*, Shape2*);
void clear(Shape2List*);

inline void
destroyShape2List(Shape2List* list)
{
  clear(list);
}

#endif // __Shape2List_h
