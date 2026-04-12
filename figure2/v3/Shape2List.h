#ifndef __Shape2List_h
#define __Shape2List_h

#include "Shape2.h"
class Shape2ListIterator
{
public:
  auto& operator ++()
  {
    _shape = _shape->_next;
    return *this;
  }

  auto operator ++(int)
  {
    auto temp = *this;
    return (_shape = _shape->_next), temp;
  }

  bool operator ==(const Shape2ListIterator& other) const
  {
    return _shape == other._shape;
  }

  bool operator !=(const Shape2ListIterator&& other) const
  {
    return !operator ==(other);
  }

  auto operator ->() const
  {
    return _shape;
  }

  auto& operator *() const
  {
    return *operator ->();
  }

  auto operator ->()
  {
    return _shape;
  }

  auto& operator *()
  {
    return *operator ->();
  }

private:
  Shape2* _shape;

  Shape2ListIterator(Shape2* shape = nullptr):
    _shape(shape)
  {
    // do nothing
  }

  friend Shape2List;

}; // Shape2ListIterator
class Shape2List
{
public:
  ~Shape2List()
  {
    clear();
  }

  void add(Shape2*);
  void remove(Shape2*);
  void clear();

  auto size() const
  {
    return _count;
  }

  auto begin() const
  {
    return Shape2ListIterator{_head};
  }

  auto end() const
  {
    return Shape2ListIterator{};
  }

  auto begin()
  {
    return Shape2ListIterator{_head};
  }

  auto end()
  {
    return Shape2ListIterator{};
  }

private:
  Shape2* _head{};
  Shape2* _tail{};
  int _count{};

  friend Figure2;
  
}; // Shape2List

#endif // __Shape2List_h
