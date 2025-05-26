#ifndef __VariadicTest_h
#define __VariadicTest_h

#include <cmath>
#include <iostream>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif // _MSC_VER

namespace tcii::tests
{ // begin namespace tcii::tests

template <bool B, typename T = void>
struct enable_if
{
  // empty
};

template <typename T>
struct enable_if<true, T>
{
  using type = T;
};

template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

namespace std_14
{ // begin namespace std_14

template <typename T>
inline constexpr auto
add(const T& t)
{
  std::cout << __PRETTY_FUNCTION__ << "\n";
  return t;
}

template <typename T, typename... Ts>
inline constexpr auto
add(const T& t, Ts&&... ts)
{
  std::cout << __PRETTY_FUNCTION__ << "\n";
  return t + add(std::forward<Ts>(ts)...);
}

} // end namespace std_14

template <typename... Ts>
inline constexpr auto
add(Ts&&... ts)
{
  std::cout << __PRETTY_FUNCTION__ << "\n";
  return (... + ts);
}

template <typename T>
inline auto
compare(const T& a, const T& b)
{
  return a == b;
}

inline auto
compare(float a, float b)
{
  return std::fabs(a - b) <= 1e-6f;
}

template <typename T>
inline auto
pairComp(const T& a, const T& b)
{
  return compare(a, b);
}

template <typename T>
inline auto
pairComp(const T&)
{
  return false;
}

template <typename T, typename... Ts>
inline auto
pairComp(const T& a, const T& b, Ts&&... ts)
{
  return compare(a, b) && pairComp(std::forward<Ts>(ts)...);
}

template <typename... Ts>
struct tuple
{
  // empty
};

template <typename T, typename... Ts>
struct tuple<T, Ts...>: tuple<Ts...>
{
  T value;

  tuple(const T& t, Ts&&... ts):
    tuple<Ts...>{std::forward<Ts>(ts)...},
    value{t}
  {
    // do nothing
  }
};

template <size_t, typename> struct _tet; // tuple element type

template <typename T, typename... Ts>
struct _tet<0, tuple<T, Ts...>>
{
  using type = T;
};

template <size_t I, typename T, typename... Ts>
struct _tet<I, tuple<T, Ts...>>
{
  using type = typename _tet<I - 1, tuple<Ts...>>::type;
};

template <size_t I, typename... Ts>
enable_if_t<I == 0, typename _tet<0, tuple<Ts...>>::type&>
get(tuple<Ts...>& t)
{
  return t.value;
}

template <size_t I, typename T, typename... Ts>
enable_if_t<I != 0, typename _tet<I, tuple<T, Ts...>>::type&>
get(tuple<T, Ts...>& t)
{
  tuple<Ts...>& base{t};
  return get<I - 1>(base);
}

} // end namespace tcii::tests

inline void
variadicTest()
{
  puts("\n**VARIADIC TEMPLATES TEST");

  using namespace tcii;
  using namespace std;

  cout << "add(2): " << tests::add(2) << '\n';
  cout << "add(2,3.14f,4,5): " << tests::add(2, 3.14f, 4, 5) << '\n';

  auto pi = 3.14f;

  cout << "add(2,pi,4,5): " << tests::add(2, pi, 4, 5) << '\n';
  cout << boolalpha << tests::pairComp(pi, float(3), 3, 4) << '\n';

  tests::tuple<float, const char*, int> t{pi, "C++", 3};

  tests::get<1>(t) = "C++20";
  cout << tests::get<0>(t) << '\n';
  cout << tests::get<1>(t) << '\n';
  cout << tests::get<2>(t) << '\n';
}

#endif // __VariadicTest_h
