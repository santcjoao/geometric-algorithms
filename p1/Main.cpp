#include "KdTree.h"
#include "Utils.h"
#include <iostream>

using namespace tcii::p1;

int
main()
{
  KdTree3<float, Points3f> tree{prand(100)};

  std::cout << tree.points().size() << '\n';
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
