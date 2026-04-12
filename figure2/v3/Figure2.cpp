#include "Figure2.h"

Shape2*
Figure2::add(Shape2* shape)
{
  if (auto p = shape->_parent; p != this)
  {
    if (p)
      p->_shapes.remove(shape);
    _shapes.add(shape);
    shape->_parent = this;
    shape->_id = ++_lastId;
    shape->color = currentColor;
    return shape;
  }
  return nullptr;
}

void
Figure2::render() const
{
  puts("**Begin figure");
  bounds().print("bounds");
  for (const auto& s : _shapes)
    s.render();
  puts("**End figure\n");
}

Bounds2
Figure2::bounds() const
{
  Bounds2 b;

  for (const auto& s : _shapes)
    b.inflate(s.bounds());
  return b;
}

void
Figure2::move(const Point2& delta)
{
  for (auto s = _lastSelected; s; s = s->_nextSelected)
    s->move(delta);
}

void
Figure2::setColor(const Color& color)
{
  for (auto s = _lastSelected; s; s = s->_nextSelected)
    s->color = color;
}

void
Figure2::clearSelection()
{
  while (_lastSelected)
  {
    auto temp = _lastSelected;

    _lastSelected = temp->_nextSelected;
    _shapes.remove(temp);
    delete temp;
  }
}

Shape2*
Figure2::select(int id)
{
  for (auto& s : _shapes)
    if (s._id == id)
    {
      if (!s._selected)
      {
        s._selected = true;
        s._nextSelected = _lastSelected;
        _lastSelected = &s;
      }
      return &s;
    }
  return nullptr;
}

void
Figure2::selectAll()
{
  _lastSelected = nullptr;
  for (auto& s : _shapes)
    {
      s._selected = true;
      s._nextSelected = _lastSelected;
      _lastSelected = &s;
    }   
}

void
Figure2::deselectAll()
{
  while (_lastSelected)
  {
    auto temp = _lastSelected;

    _lastSelected = temp->_nextSelected;
    temp->_nextSelected = nullptr;
    temp->_selected = false;
  }
}
