#include "Figure2.h"

int
main()
{
  Figure2 f = createFigure2();
  Point2 p1{100, 100};
  Point2 p2{300, 300};
  Point2 p3{400, 400};
  Shape2* s1 = addLine(&f, p1, p2);

  addRectangle(&f, p1, p3);
  addCircle(&f, p3, 100);

  Shape2* s4 = addLine(&f, p2, p3);

  select(&f, s1->id);
  select(&f, s4->id);
  render(&f);
  clearSelection(&f);
  selectAll(&f);
  setColor(&f, Color{1, 1, 1});
  deselectAll(&f);
  render(&f);
  destroyFigure2(&f);
  //puts("Press any key to exit...");
  //getchar();
  return 0;
}
