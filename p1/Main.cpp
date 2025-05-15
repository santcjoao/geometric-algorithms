#include "KdTree.h"
#include "Utils.h"
#include <iostream>

using namespace tcii::p1;

int
main()
{
  constexpr size_t D{3};
  using R = float;
  using A = PointVector<D, R>;

  constexpr size_t np{100};
  KdTree<D, R, A> tree{prand<D, R>(np)};
  auto& points = tree.points();
  auto& bounds = tree.bounds();

  std::cout.precision(3);
  std::cout << "Tree bounds: " << bounds << '\n';
  std::cout << "Points: " << points.size() << '\n';

  R d{};

  for (size_t i = 0; i < np; ++i)
    d += distance(points[i], bounds);
  std::cout << "Distance sum: " << d << '\n';
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
