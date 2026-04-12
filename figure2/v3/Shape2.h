#ifndef __Shape2_h
#define __Shape2_h

#include "Bounds2.h"
#include "Color.h"

class Figure2;
class Shape2List;
class Shape2ListIterator;

class Shape2
{
public:
  Color color;

  virtual ~Shape2() = default;

  auto parent() const
  {
    return _parent;
  }

  auto id() const
  {
    return _id;
  }

  virtual void render() const;
  virtual Bounds2 bounds() const = 0;
  virtual void move(const Point2&) = 0;

protected:
  Shape2() = default;

  virtual void beginRender() const;
  virtual void endRender() const;
  virtual void draw() const = 0;

private:
  Figure2* _parent{};
  int _id{};
  Shape2* _next;
  Shape2* _previous;
  Shape2* _nextSelected;
  bool _selected;

  friend Figure2;
  friend Shape2List;
  friend Shape2ListIterator;

}; // Shape2

#endif // __Shape2_h
