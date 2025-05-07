#ifndef __FuncTest_h
#define __FuncTest_h

#include <string>

namespace tcii::tests
{ // begin namespace tcii::tests

// A function base template
template <typename T>
inline constexpr auto
max(const T& a, const T& b)
{
  return a > b ? a : b;
}

// Specialization for string
template <>
inline auto
max<std::string>(const std::string& a, const std::string& b)
{
  return a.size() > b.size() ? a : b;
}

// Overload for int
inline constexpr auto
max(int a, int b)
{
  return a > b ? a : b;
}

// Another (abbreviated) function base template. Same as
// template <typename T, typename U>
// inline constexpr auto
// min(const T& a, const U& b) { ... }
inline constexpr auto
min(const auto& a, const auto& b)
{
  return a < b ? a : b;
}

// Yet another function base template
template <int N>
inline auto
tfat()
{
  return N * tfat<N - 1>();
}

// Specialization for 1
template <>
inline auto
tfat<1>()
{
  return 1;
}

// A constexpr function works
inline constexpr auto
cfat(int n) -> decltype(n)
{
  return n <= 1 ? 1 : n * cfat(n - 1);
}

// A variable template
template <typename T>
constexpr auto pi = static_cast<T>(3.14159);

} // end namespace tcii::tests

inline void
funcTest()
{
  puts("\n**FUNCTION TEMPLATES/CONSTEXPR FUNCTIONS");

  using namespace tcii::tests;

  constexpr auto i = 2;
  int j{5};

  // Runtime evaluation
  printf("max(i,j): %d\n", max<int>(i, j));
  printf("min(j,pi): %f\n", min(j, pi<float>));
  printf("fat(j): %d\n", cfat(j));
  // Compile-time evaluation
  printf("min(i,pi): %f\n", min(i, pi<float>));
  printf("max(2,3): %d\n", max<int>(2, 3));
  printf("fat(5): %d\n", tfat<5>());
  printf("fat(3): %d\n", cfat(3));
  printf("fat(i): %d\n", cfat(i));
}

#endif // __FuncTest_h
