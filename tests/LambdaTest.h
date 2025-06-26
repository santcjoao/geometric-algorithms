#ifndef __LambdaTest_h
#define __LambdaTest_h

#include <iostream>
#include <functional>
#include <string>

namespace tcii::tests
{ // begin namespace tcii::tests

template <typename... Args>
void
varprint(Args&&... args)
{
  (std::cout << ... << std::forward<Args>(args)) << '\n';
}

template <typename... Args>
using varfunc = std::function<void(Args&&...)>;

template <typename... Args>
class Functor
{
public:
  Functor(const char* name):
    _name{name}
  {
    // do nothing
  }

  void operator ()(Args&&... args) const
  {
    varprint(_name, ':', std::forward<Args>(args)...);
  }

private:
  std::string _name;

}; // Functor

template <typename F, typename... Args>
inline void
call(F&& f, Args&&... args)
{
  f(std::forward<Args>(args)...);
}

} // end namespace tcii::tests

inline void
lambdaTest()
{
  puts("\n**FUNCTOR/LAMBDA/FOLD EXPRESSION");

  using namespace tcii::tests;

  const char* name = "test";

  Functor<int, int, int> f{name};
  auto fp = varfunc<int, int, int>{f};
  auto lf = [name](int i, int j, int k)
    {
      varprint(name, ':', i, j, k);
    };

  f(1, 2, 3);
  call(fp, 1, 2, 3);
  fp = lf;
  call(fp, 1, 2, 3);
}

#endif // __LambdaTest_h
