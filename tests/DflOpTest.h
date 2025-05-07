#ifndef __DflOpTest_h
#define __DflOpTest_h

#include <cstdio>

namespace tcii::tests
{ // begin namespace tcii::tests

class Aux
{
public:
  ~Aux()
  {
    puts("**Dtor");
  }

  Aux()
  {
    puts("**Default ctor");
  }

  Aux(const Aux&)
  {
    puts("**Copy ctor");
  }

  Aux(Aux&&) noexcept
  {
    puts("**Move ctor");
  }

  Aux& operator =(const Aux&)
  {
    puts("**Copy op");
    return *this;
  }

  Aux& operator =(Aux&&) noexcept
  {
    puts("**Move op");
    return *this;
  }

}; // Aux

class W
{
public:
  ~W() = default;
  W() = default;
  W(const W&) = default;
  W& operator =(const W&) = default;
  W(W&&) = default;
  W& operator =(W&&) = default;

  void print(const char* s) const
  {
    printf("**W:%s**\n", s);
  }

private:
  Aux _aux;

}; // W

inline auto
makeW()
{
  W temp;

  temp.print("label");
  return temp;
}

} // end namespace tcii::tests

inline void
dflOpTest()
{
  puts("**\nSPECIAL MEMBER FUNCTIONS");

  using namespace tcii::tests;

  W w1;

  w1.print("w1");

  auto w2 = w1;

  w2.print("w2");

  auto w3 = makeW();

  w3.print("w3");
  w1 = w2;
  w1.print("w1");
  w2 = w3;
  w2.print("w2");
}

#endif // __DflOpTest_h
