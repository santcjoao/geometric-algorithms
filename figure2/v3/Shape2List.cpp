#include "Shape2List.h"

void
Shape2List::add(Shape2* shape)
{
  shape->_nextSelected = shape->_next = nullptr;
  shape->_selected = false;
  if (!_head)
    _head = shape;
  else
  {
    _tail->_next = shape;
    shape->_previous = _tail;
  }
  _tail = shape;
  _count++;
}

void
Shape2List::remove(Shape2* shape)
{
  if (shape->_previous)
    shape->_previous->_next = shape->_next;
  if (shape->_next)
    shape->_next->_previous = shape->_previous;
  if (shape == _head)
    _head = shape->_next;
  if (shape == _tail)
    _tail = shape->_previous;
  _count--;
}

void
Shape2List::clear()
{
  while (_head)
  {
    auto temp = _head;

    _head = temp->_next;
    delete temp;
  }
  _tail = nullptr;
  _count = 0;
}
