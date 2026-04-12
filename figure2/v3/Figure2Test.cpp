#include "Circle2.h"
#include "Figure2.h"
#include "Line2.h"
#include "Rectangle2.h"

inline auto
addLine(Figure2& f, const Point2& p1, const Point2& p2)
{
  return f.add(new Line2{p1, p2});
}

inline auto
addRectangle(Figure2& f, const Point2& p1, const Point2& p2)
{
  return f.add(new Rectangle2{p1, p2});
}

inline auto
addCircle(Figure2& f, const Point2& center, int radius)
{
  return f.add(new Circle2{center, radius});
}

int
main()
{
  Figure2 f;
  Point2 p1{100, 100};
  Point2 p2{300, 300};
  Point2 p3{400, 400};
  auto s1 = addLine(f, p1, p2);

  addRectangle(f, p1, p3);
  addCircle(f, p3, 100);

  auto s4 = addLine(f, p2, p3);

  f.select(s1->id());
  f.select(s4->id());
  f.render();
  f.clearSelection();
  f.selectAll();
  f.setColor({1, 1, 1});
  f.deselectAll();
  f.render();
  //puts("Press any key to exit...");
  //getchar();
  return 0;
}
