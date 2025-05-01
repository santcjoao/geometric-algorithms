#include "Expression.h"

int
main() try
{
  using namespace tcii::dcalc;

  auto c = Expression{2} + (Expression{3} + 1);
  auto t = Expression::term(3) + 0;
  auto m = Expression::term(1, -1);

  /*
  std::cout << c << " (6)\n";
  std::cout << c + c << " (12)\n";
  std::cout << t << " (x^3)\n";
  std::cout << t + t << " (2x^3)\n";
  std::cout << t + m + m << " (x^3-2x)\n";
  std::cout << x() + m << " (0)\n";
  */

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
  std::cout << c * c << " (36)\n";
  std::cout << t * c << " (6x^3)\n";
  std::cout << c * t << " (6x^3)\n";
  std::cout << t * 1 << " (x^3)\n";
  std::cout << t * t << " (x^6)\n";
  std::cout << t * t * x() * 2 << " (2x^7)\n";
  std::cout << a1 * x() << " (6x+x^4)\n";
  std::cout << x() * a1 << " (6x+x^4)\n";
  std::cout << t * (a1 + a2) << " (12x^3+2x^6)\n";
  std::cout << (a1 + a2) * t << " (12x^3+2x^6)\n";
  std::cout << (a1 * a2) * t << " (12x^6+x^9+36x^3)\n";
  std::cout << t * (a1 * a2) << " (12x^6+x^9+36x^3)\n";

  auto a4 = (a1 + a2) * (a1 + a2);
  auto a5 = (a1 * a2) + (a1 * a2);
  auto a6 = (a1 * a2) * (a1 * a2);

  std::cout << a4 << " (144+48x^3+4x^6)\n";
  std::cout << a4.diff() << " (144x^2+24x^5)\n";
  std::cout << a5 << " (24x^3+2x^6+72)\n";
  std::cout << a1 * a2 + x() << " (12x^3+x^6+36+x)\n";
  std::cout << x() + a1 * a2 << " (x+12x^3+x^6+36)\n";
  std::cout << a6 << " (216x^6+24x^9+864x^3+x^12+1296)\n";
  std::cout << a6.diff() << " (1296x^5+216x^8+2592x^2+12x^11)\n";
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
catch (const std::exception& e)
{
  std::cout << e.what() << '\n';
  return 1;
}
