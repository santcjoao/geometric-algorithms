#include "Shape2List.h"

void
add(Shape2List* list, Shape2* shape)
{
  shape->nextSelected = shape->next = nullptr;
  shape->selected = false;
  if (list->head == nullptr)
    list->head = shape;
  else
  {
    list->tail->next = shape;
    shape->previous = list->tail;
  }
  list->tail = shape;
  list->count++;
}

void
remove(Shape2List* list, Shape2* shape)
{
  if (shape->previous)
    shape->previous->next = shape->next;
  if (shape->next)
    shape->next->previous = shape->previous;
  if (shape == list->head)
    list->head = shape->next;
  if (shape == list->tail)
    list->tail = shape->previous;
  list->count--;
}

void
clear(Shape2List* list)
{
  while (list->head != nullptr)
  {
    auto temp = list->head;

    list->head = temp->next;
    delete temp;
  }
  list->tail = nullptr;
  list->count = 0;
}
