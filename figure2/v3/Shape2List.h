#ifndef __Shape2List_h
#define __Shape2List_h

#include "Shape2.h"
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

private:
  Shape2* _head{};
  Shape2* _tail{};
  int _count{};

  friend Figure2;
  
}; // Shape2List

#endif // __Shape2List_h
