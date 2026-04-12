#include "Shape2.h"
#include <typeinfo>

void
Shape2::render() const
{
  beginRender();
  draw();
  endRender();
}

void
Shape2::beginRender() const
{
  printf("**%s %d%s\n",
    typeid(*this).name(),
    _id,
    _selected ? " (selected)" : "");
}

void
Shape2::endRender() const
{
  bounds().print("bounds");
  color.print("color");
}
