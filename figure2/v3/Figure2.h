#ifndef __Figure2_h
#define __Figure2_h

#include "Shape2List.h"

class Figure2
{
public:
  Color currentColor{Color::black};

  void clear()
  {
    _shapes.clear();
    _lastSelected = nullptr;
    _lastId = 0;
  }

  void render() const;
  Bounds2 bounds() const;

  Shape2* add(Shape2*);
  void move(const Point2&);
  void setColor(const Color&);
  void clearSelection();
  Shape2* select(int);
  void selectAll();
  void deselectAll();

private:
  Shape2List _shapes;
  Shape2* _lastSelected{};
  int _lastId{};

}; // Figure2

#endif // __Figure2_h
