#ifndef __InitTest_h
#define __InitTest_h

#include <cstdio>

namespace tcii::tests
{ // begin namespace tcii::tests

template <typename S>
inline void
println(const S& s, const char* label)
{
  s.print(label);
  putchar('\n');
}

struct S1
{
  int a;
  int b;
  int c;

  void print(const char* label) const
  {
    printf("%s S1::a:%d S1::b:%d S1::c:%d", label, a, b, c);
  }

}; // S1

struct S2
{
  int a;
  int b{-9};
  int c;

  void print(const char* label) const
  {
    printf("%s S2::a:%d S2::b:%d S2::c:%d", label, a, b, c);
  }

}; // S2

struct S3
{
  int a;
  int b{-9};
  int c;

  S3()
  {
    printf("S3::S3() this:%p\n", this);
  }

  void print(const char* label) const
  {
    printf("%s S3::a:%d S3::b:%d S3::c:%d", label, a, b, c);
  }

}; // S3

struct S4
{
  int a;
  int b{-9};
  int c;

  S4() = default; // explicitly defaulted

  void print(const char* label) const
  {
    printf("%s S4::a:%d S4::b:%d S4::c:%d", label, a, b, c);
  }

}; // S4

struct D1: S1
{
  int a;
  int d;

  void print(const char* label) const
  {
    S1::print(label);
    printf(" D1::a:%d D1::d:%d", a, d);
  }

}; // D1

struct D2: D1, S2
{
  int a;
  int d{-2};

  void print(const char* label) const
  {
    D1::print(label);
    printf(" S2::a:%d S2::b:%d S2::c:%d D2::a:%d D2::d:%d",
      S2::a, S2::b, S2::c,
      a, d);
  }

}; // D2

struct D3: S3
{
  int a;
  int d{-2};

  void print(const char* label) const
  {
    S3::print(label);
    printf(" D3::a:%d D3::d:%d", a, d);
  };

}; // D3

struct D4: D3, S4
{
  int a;
  int d{-2};

  // Try commenting out the line below
  D4() = default;

  void print(const char* label) const
  {
    D3::print(label);
    printf(" S4::a:%d S4::b:%d S4::c:%d D4::a:%d D4::d:%d",
      S4::a, S4::b, S4::c,
      a, d);
  };

}; // D4

} // end namespace tcii::tests

inline void
initTest()
{
  puts("\n**OBJECT INITIALIZATION");

  using namespace tcii::tests;

  {
    // Scalar type
    int a{3};
    int b{};
    int c{-5};

    printf("a:%d b:%d c:%d\n", a, b, c);
  }
  {
    // Aggregate (no explicitly-defined constructors)
    S1 s1_1;
    S1 s1_2{};
    S1 s1_3{-1};
    S1 s1_4{1, 2};
    S1 s1_5{.a = 3, .c = 4}; // designated initializers

    println(s1_1, "s1_1");
    println(s1_2, "s1_2");
    println(s1_3, "s1_3");
    println(s1_4, "s1_4");
    println(s1_5, "s1_5");
  }
  {
    // Aggregate (with default member initialization)
    S2 s2_1;
    S2 s2_2{};
    S2 s2_3{-1};
    S2 s2_4{1, 2};
    S2 s2_5{.a = 3, .c = 4}; // designated initializers

    println(s2_1, "s2_1");
    println(s2_2, "s2_2");
    println(s2_3, "s2_3");
    println(s2_4, "s2_4");
    println(s2_5, "s2_5");
  }
  {
    // Class (with an explicitly-defined constructor)
    S3 s3_1;
    S3 s3_2{};
    S4 s4_1;
    S4 s4_2{};

    println(s3_1, "s3_1");
    println(s3_2, "s3_2");
    println(s4_1, "s4_1");
    println(s4_2, "s4_2");
  }
  // Aggregate with bases
  {
    D1 d1_1;
    D1 d1_2{};
    D1 d1_3{-1};
    D1 d1_4{1, 2};
    D1 d1_5{1, 2, 3, 4, 5};
    D1 d1_6{.a = -1};

    println(d1_1, "d1_1");
    println(d1_2, "d1_2");
    println(d1_3, "d1_3");
    println(d1_4, "d1_4");
    println(d1_5, "d1_5");
    println(d1_6, "d1_6");
  }
  {
    D2 d2_1;
    D2 d2_2{};
    D2 d2_3{-1};
    D2 d2_4{1, 2};
    D2 d2_5{1, 2, 3, 4, 5, 6, 7, 8, 9};
    D2 d2_6{.a = -1, .d = 3};

    println(d2_1, "d2_1");
    println(d2_2, "d2_2");
    println(d2_3, "d2_3");
    println(d2_4, "d2_4");
    println(d2_5, "d2_5");
    println(d2_6, "d2_6");
  }
  // Class with bases
  {
    D3 d3_1;
    D3 d3_2{};
    D4 d4_1;
    D4 d4_2{};

    println(d3_1, "d3_1");
    println(d3_2, "d3_2");
    println(d4_1, "d4_1");
    println(d4_2, "d4_2");
  }
}

#endif // __InitTest_h
