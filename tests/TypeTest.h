#ifndef __TypeTest_h
#define __TypeTest_h

#include "graphics/Vec3.h"
#include <iostream>

namespace tcii::tests
{ // begin namespace tcii::tests

template <typename T>
inline constexpr void
printTypeInfo()
{
  using namespace std;

  cout << "Type: " << typeid(T).name() << '\n';
  cout << "Size: " << sizeof(T) << '\n';
}

template <typename T>
inline constexpr void
printTypeInfo(const T& value)
{
  using namespace std;

  printTypeInfo<T>();
  if constexpr (is_arithmetic_v<T>)
  {
    cout << "Min: " << numeric_limits<T>::min() << '\n';
    cout << "Max: " << numeric_limits<T>::max() << '\n';
    cout << "Val: " << value << "\n---\n";
  }
}

inline void
numericTypeTest()
{
  puts("\n**ARITHMETIC TYPES");
  printTypeInfo<bool>();
  printTypeInfo<char>();
  printTypeInfo<signed char>();
  printTypeInfo<unsigned char>();
  printTypeInfo<wchar_t>();
  printTypeInfo<short>();
  printTypeInfo<unsigned short>();
  printTypeInfo<int>();
  printTypeInfo<unsigned int>();
  printTypeInfo<long>();
  printTypeInfo<unsigned long>();
  printTypeInfo<long long>();
  printTypeInfo<unsigned long long>();
  printTypeInfo<float>();
  printTypeInfo<double>();
  printTypeInfo<long double>();

  const auto b = true;
  const auto c = 'c';
  const signed char sc{'c'};
  const unsigned char uc{63U};
  const auto w = L'c';
  const short s{1};
  const unsigned short us{1U};
  const auto i = 1;
  const auto ui = 1U;
  const auto l = 1L;
  const auto ul = 1UL;
  const auto ll = 1LL;
  const auto ull = 1ULL;
  const auto f = 3.14F;
  const auto d = 314.0e-2;
  const auto ld = 3.14L;

  puts("\n**ARITHMETIC LITERALS");
  printTypeInfo(b);
  printTypeInfo(c);
  printTypeInfo(sc);
  printTypeInfo(uc);
  printTypeInfo(s);
  printTypeInfo(us);
  printTypeInfo(i);
  printTypeInfo(ui);
  printTypeInfo(l);
  printTypeInfo(ul);
  printTypeInfo(ll);
  printTypeInfo(ull);
  printTypeInfo(f);
  printTypeInfo(d);
  printTypeInfo(ld);
}

template <typename real>
struct Matrix33
{
  cg::Vec3<real> v0;
  cg::Vec3<real> v1;
  cg::Vec3<real> v2;
};

template <typename real>
std::ostream&
operator <<(std::ostream& os, const Matrix33<real>& m)
{
  os << m.v0 << '\n' << m.v1 << '\n' << m.v2 << '\n';
  return os;
}

inline void
structTypeTest()
{
  using namespace cg;
  using Matrix33f = Matrix33<float>;

  puts("\n**STRUCT (POD) INITIALIZATION");

  // Initialize using initializer list ('=' is not needed)
  Vec3f p{1, 2, 3};
  Vec3f q{};

  printTypeInfo<Vec3f>();
  std::cout << 'p' << p << '\n';
  std::cout << 'q' << q << "\n---\n";

  Matrix33f a{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  Matrix33f z{};

  printTypeInfo<Matrix33f>();
  std::cout << "a\n" << a;
  std::cout << "z\n" << z << "---\n";

  struct X
  {
    struct Inner
    {
      Vec3f p;
      Vec3f q;
    };

    Inner a;
    float f;
  };

  X x{{{1, 2, 3}, {4, 5, 6}}, 7};

  printTypeInfo(x);
  std::cout << "x.a.p" << x.a.p << '\n';
  std::cout << "x.a.q" << x.a.q << '\n';
  std::cout << "x.f: " << x.f << "\n---\n";

  struct Y
  {
    X::Inner x;
    int a;
  };

  printTypeInfo<Y>();
}

using uint = unsigned;

// Arrays cannot be function parameters
inline void
f(int a[10])
{
  puts("int[10] decays to int*:");
  printTypeInfo(a);
}

inline void
f(int a[][2])
{
  puts("int[][2] decays to int(*)[2]:");
  printTypeInfo(a);
}

inline void
arrayTest()
{
  puts("\n**ARRAY INITIALIZATION");

  using IA10 = int[10];

  IA10 ia10_1;
  // Arrays are not copied
  auto p = ia10_1; // decays to int*

  printTypeInfo(p);
  *p = 1; // (p[0] = 1) == (ia10_1[0] = 1)

  int ia10_2[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int ia3_1[3]{1, 2, 3};
  int ia3_2[3]{1};

  using IA2x3 = int[2][3];

  IA2x3 ia2x3_1;
  auto q = ia2x3_1; // decays to int(*)[3]

  printTypeInfo(q);
  *q[0] = 1; // (q[0][0] = 1) == (ia2x3_1[0][0] = 1)

  int ia3x2_1[][2]{1, 2, 3, 4, 5};
  int ia3x2_2[][2]{{1, 2}, {3, 4}, {5}};

  f(ia10_1);
  f(ia3x2_1);

  using IntTable = uint[]; // incomplete type

  // Types defined by initilization
  IntTable f1{1, 2};
  IntTable f2{1, 2, 3};

  printTypeInfo(f1);
  printTypeInfo(f2);
}

// Func is a function, not a function pointer (int(*)(uint))
using Func = uint(uint);

inline uint
f(uint a) // f is a Func
{
  return a;
}

// Functions cannot be function parameters
inline void
funcFuncTest(Func f)
{
  puts("\n**FUNCTION AS FUNCTION PARAMETER");
  puts("Func decays to int(*)(uint):");
  printTypeInfo(f);
  f(2); // (*f)(2);
}

} // end namespace tcii::tests

inline void
typeTest()
{
  using namespace tcii::tests;

  numericTypeTest();
  structTypeTest();
  arrayTest();
  funcFuncTest(f);
}

#endif // __TypeTest_h
