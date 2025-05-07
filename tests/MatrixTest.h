#include "math/Matrix.h"
#include <iostream>

namespace tcii::tests
{ // bengin namespace tcii::tests

inline void
print(const char* label, const math::Vector<float>& x)
{
  for (int i = 0; i < x.size(); ++i)
    printf("%s(%d)=%g\n", label, i, x(i));
}

inline void
print(const math::Matrix<float>::Element& e)
{
  printf("(%d,%d)=%g\n", e.i, e.j, e.value);
}

} // end namespace tcii::tests

inline void
vectorTest()
{
  using namespace tcii::tests;

  puts("\n**VECTOR TEST**");

  auto u = math::Vector<float>{10};

  for (int i = 0; i < u.size(); ++i)
    u(i) = float(i);

  auto c = u.clone();
  auto v = u;
  math::Vector<float> q;

  q = u += 2.0f * c;
  print("q", q);
  print("v", v);
  print("(q+[])", q + math::Vector<float>{}); // error
}

inline void
matrixTest()
{
  using namespace tcii::tests;

  puts("\n**MATRIX TEST**");

  auto a = math::Matrix<float>{3, 3};
  auto b = math::Matrix<float>{3, 3};

  for (int i = 0; i < a.rows(); ++i)
    for (int j = 0; j < a.cols(); ++j)
      a(i, j) = b(i, j) = float(i + j);

  auto c = a + b;
  auto v = math::Vector<float>{3};

  for (int i = 0; i < c.rows(); ++i)
  {
    std::cout << "c(" << i << ",:) ";
    for (int j = 0; j < c.cols(); ++j)
      std::cout << c(i, j) << ' ';
    std::cout << '\n';
    v(i) = c(i, i);
  }
  print("c*v", c * v);
  // copy op
  a = c;
  c.iterate(print);
  // move op
  b = std::move(c);
  b.iterate(print);
}
