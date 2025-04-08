#ifndef __TypeTest_h
#define __TypeTest_h

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

#define ASSERT_REAL(T, msg) static_assert(std::floating_point<T>, msg)

template <typename T>
struct Vec3
{
  ASSERT_REAL(T, "Vec3: floating-point type expected");

  T x;
  T y;
  T z;
};

template <typename T>
std::ostream&
operator <<(std::ostream& os, const Vec3<T>& v)
{
  os << '(' << v.x << ',' << v.y << ',' << v.z << ')' <<  '\n';
  return os;
}

template <typename T>
struct Matrix33
{
  Vec3<T> v0;
  Vec3<T> v1;
  Vec3<T> v2;
};

template <typename T>
std::ostream&
operator <<(std::ostream& os, const Matrix33<T>& m)
{
  os << m.v0 << m.v1 << m.v2;
  return os;
}

inline void
structTypeTest()
{
  using Vec3f = Vec3<float>;
  using Matrix33f = Matrix33<float>;

  puts("\n**STRUCT INITIALIZATION");

  // Initialize using initializer list ('=' is not needed)
  Vec3f p{1, 2, 3};
  Vec3f q{};

  printTypeInfo<Vec3f>();
  std::cout << 'p' << p;
  std::cout << 'q' << q << "---\n";

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
  std::cout << "x.a.p" << x.a.p;
  std::cout << "x.a.q" << x.a.q;
  std::cout << "x.f: " << x.f << "\n---\n";

  struct Y
  {
    X::Inner x;
    int a;
  };

  printTypeInfo<Y>();
}

} // end namespace tcii::tests

inline void
typeTest()
{
  using namespace tcii::tests;

  numericTypeTest();
  structTypeTest();
}

#endif // __TypeTest_h
