#ifndef __InitTest_h
#define __InitTest_h

#include <cstdio>

namespace tcii::tests
{ // begin namesapce tcii::tests

struct S1
{
  int a;
  int b;
  int c;

  void print(const char* label) const
  {
    printf("%s a:%d b:%d c:%d\n", label, a, b, c);
  }

}; // S1

struct S2
{
  int a;
  int b{-9};
  int c;

  void print(const char* label) const
  {
    printf("%s a:%d b:%d c:%d\n", label, a, b, c);
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
    printf("%s a:%d b:%d c:%d\n", label, a, b, c);
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
    printf("%s a:%d b:%d c:%d\n", label, a, b, c);
  }

}; // S4

} // end namesapce tcii::tests

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

    s1_1.print("s1_1");
    s1_2.print("s1_2");
    s1_3.print("s1_3");
    s1_4.print("s1_4");
    s1_5.print("s1_5");
  }
  {
    // Aggregate (with default member initialization)
    S2 s2_1;
    S2 s2_2{};
    S2 s2_3{-1};
    S2 s2_4{1, 2};
    S2 s2_5{.a = 3, .c = 4}; // designated initializers

    s2_1.print("s2_1");
    s2_2.print("s2_2");
    s2_3.print("s2_3");
    s2_4.print("s2_4");
    s2_5.print("s2_5");
  }
  {
    // Class (with an explicitly-defined constructor)
    S3 s3_1;
    S3 s3_2{};
    S4 s4_1;
    S4 s4_2{};

    s3_1.print("s3_1");
    s3_2.print("s3_2");
    s4_1.print("s4_1");
    s4_2.print("s4_2");
  }
}

#endif // __InitTest_h
