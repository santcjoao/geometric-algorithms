#include "Expression.h"

int
main() try
{
  using namespace tcii::dcalc;

  auto t = Expression::term(2);

  t += t;
  std::cout << t << '\n';

  Expression e{2};

  e += x() + t + 3;
  std::cout << e << '\n' << e.diff() << '\n';
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
catch (const std::exception& e)
{
  std::cout << e.what() << '\n';
  return 1;
}
