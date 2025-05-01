#include "Expression.h"

int
main() try
{
  using namespace tcii::dcalc;

  /*
  auto t = Expression::term(2);

  t += t + t;

  auto e = Expression{3} + x() + t + x() + 5;

  std::cout << e << '\n';
  t = x() + t + x() + 1;
  std::cout << t << '\n';
  e += t;
  std::cout << e << '\n' << e.diff() << '\n';
  */
  auto c = Expression{2} + (Expression{3} + 1);
  auto t = Expression::term(3) + 0;
  auto m = Expression::term(1, -1);

  std::cout << c << " (6)\n";
  std::cout << c + c << " (12)\n";
  std::cout << t << " (x^3)\n";
  std::cout << t + t << " (2x^3)\n";
  std::cout << t + m + m << " (x^3-2x)\n";
  std::cout << x() + m << " (0)\n";

  auto a1 = c + t;
  auto a2 = t + c;

  std::cout << a1 << " (6+x^3)\n";
  std::cout << a2 << " (x^3+6)\n";
  std::cout << a1 + c << " (12+x^3)\n";
  std::cout << c + a1 << " (12+x^3)\n";
  std::cout << a2 + t << " (2x^3+6)\n";
  std::cout << t + a2 << " (2x^3+6)\n";
  std::cout << t + (a1 + x()) << " (6+2x^3+x)\n";
  std::cout << (t + a1) + x() << " (6+2x^3+x)\n";
  std::cout << a1 + a2 << " (12+2x^3)\n";
  std::cout << (a1 + a2) + (a2 + a1) << " (24+4x^3)\n";

  auto a3 = (a1 + a2) + (a2 + a1) + (a1 + x());

  std::cout << a3 << " (30+5x^3+x)\n";
  std::cout << a3.diff() << " (15x^2+1)\n";
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
catch (const std::exception& e)
{
  std::cout << e.what() << '\n';
  return 1;
}
