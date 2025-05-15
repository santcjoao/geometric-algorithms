#ifndef __RefTest_h
#define __RefTest_h

#define _COPY_CTOR
#define _COPY_OP
#define _MOVE_CTOR
#define _MOVE_OP

#include <cstdio>
#include <string>

// About lvalue and rvalue references and value categories in C++:
// A brief by Paulo Pagliosa

/*
Roughly speaking, an lvalue is an expression, E, from which we can
take the address, ie, &E is a valid expression. An rvalue, in turn,
is a constant expression or an expression whose evaluation results
in a temporary object (eg, the value of a binary expression or the
return value of a function call).

A reference is an alias for an object resulting from the evaluation
of an expression. An lvalue reference is one that is bound to an
lvalue, whereas an rvalue reference is bound to a constant or
temporary object. Period. The declaration of a variable whose type
is an lvalue (resp. rvalue) reference requires an initializer that
specifies the lvalue (resp. rvalue) to which the reference is bound.

For example, consider the following class:
*/

namespace tcii::tests
{ // begin namespace tcii::tests

class X: public std::string
{
public:
  using super = std::string;

  X(const char* const s):
    super{s}
  {
    print("**Ctor(literal)");
  }

  X(std::string&& s):
    super{std::move(s)}
  {
    print("**Ctor(string)");
  }

  X()
  {
    print("**Default ctor");
  }

#ifdef _COPY_CTOR
  X(const X& x):
    super{x}
  {
    print("**Copy ctor");
  }
#endif

#ifdef _MOVE_CTOR
  X(X&& x) noexcept:
    super{std::move(x)}
  {
    print("**Move ctor");
  }
#endif

  ~X()
  {
    print("**Dtor");
  }

#ifdef _COPY_OP
  X& operator =(const X& x)
  {
    super::operator =(x);
    print("**Copy op");
    return *this;
  }
#endif

#ifdef _MOVE_OP
  X& operator =(X&& x) noexcept
  {
    super::operator =(std::move(x));
    print("**Move op");
    return *this;
  }
#endif

  X operator +(const X& x) const
  {
    return std::operator +(*this, x);
  }

  void print(const char* const s) const
  {
    printf("%s: \"%s_%d\"\n", s, c_str(), _id);
  }

private:
  static inline int _nextId;
  int _id{++_nextId};

}; // X

} // end namespace tcii::tests

/*
Considering the class X in the namespace tcii::tests and the code
below, the expression X{"C++"} is an rvalue. (Indeed, the expression
results in a temporary object: attempting to get the address of such
an object, i.e., &X{"C++"}, causes a compilation error.) Therefore,
it is not correct to write:

X& r1 = X{"C++"}; // ERROR (1)

(Since r1 is an lvalue reference, it can only be initialized with an
lvalue.) However, it is valid to initialize a *const* lvalue
reference with an rvalue:

const X& r2 = X{"C++"}; // (2)
const int& r3 = 3; // (3)

In declaration (3), the compiler creates a temporary object to keep
the literal rvalue (more specifically, a prvalue, or pure rvalue).
The lifetime of the temporary is defined by the scope of r3.

Of course, an rvalue reference can be initialized with an rvalue:

X&& r4 = X{"C++"}; // (4)
std::string&& r5 = "C++"; // (5)

By using r4 (but not r2), it is possible to modify the temporary:

r4[1] = r4[2] = r5[1] = r5[2] = '9';
r2.print("r2");
r4.print("r4");

The output is:

r2: "C++_1"
r4: "C99_2"

Also, an rvalue can be used as initializer of an object:

X x1 = X{"C++"}; // (6)
X x2 = "C99"; // (7)

Declaration (6) statically creates an object of the type X named x1.
(Only the first constructor declared in the class X in invoked here.
There is no copying since the code generated for the constructor
operates directly on the object x1). The expression x1 is clearly an
lvalue (we can get its address, ie, &x1). Therefore, we can not
declare:

X&& r6 = x1; // ERROR (8)

However, it is correct to write:

auto&& r7 = x1; // OK: r7 is an lvalue reference (9)
X& r8 = r7; // (10)
auto x3 = r8; // (11)
X&& r9 = static_cast<X&&>(r8); // (12)
auto x4 = std::move(r8); // (13)

(We will discuss the type deductions in declarations (9) and (13)
later.) Declaration (11) statically creates an object of the type
X named x3 as a copy of the object referenced by r8 (i.e., x1).
Declaration (12) initializes an rvalue reference with the rvalue
resulting from a cast expression. Note that r8 is an lvalue, but
the cast expression is an rvalue (more specifically, a xvalue, or
"expiring" value). Thus, even an object with identity (x1, in this
case) can be considered "temporary" and used to initialize an rvalue
reference. The std::move function does exactly this cast.

In fact, *every* variable in an expression is an lvalue, be it an
object, an lvalue reference, or even an rvalue reference. Thus, the
expressions r8 and r4 are both lvalues (we can write &r8 and &r4),
but an lvalue reference must always be initialized with an lvalue,
whereas an rvalue reference must be initialized with an rvalue.

Once initialized, lvalue and rvalue references are used in the same
way. Consider the following assignments:

r9 = r4; // (14)
r8 = r4; // (15)
r4 = x2; // (16)
r8 = x1; // (17)
r9 = std::move(r4); // (18)
r7 = r9; // (19)
r8 = std::move(r7); // (20)
r9 = r2 + X{"20"}; // (21)

Remember that assigning a value, x, to a reference, r, does not
change the reference itself, but the object to which the reference
is bound. Also, if x is an lvalue, then it is *copied* to r (more
specifically, copied to the object which r is bound); otherwise, if
it is an rvalue, then it is *moved* to r (ie, the object which r is
bound). Thus, in expressions (14)-(17) and (19), the assignment
invokes the copy operator (which takes as parameter a const lvalue
reference), and in expressions (18) and (20)-(21), the move operator
(which takes as parameter an rvalue reference, i.e., a reference for
a temporary). (Attempting to use r8 after expression (20) should
cause a compiler warning (at least) since x1 has been moved, i.e.,
it is supposed that its properties have been "stolen" by other
object, although the state of the object is still valid.)
*/

namespace tcii::tests
{ // begin namespace tcii::tests

template <typename T>
inline void
vcat(const char* s, [[maybe_unused]] T& v)
{
  printf("%s: lvalue\n", s);
}

template <typename T>
inline void
vcat(const char* s, [[maybe_unused]] const T& v)
{
  printf("%s: const lvalue\n", s);
}

template <typename T>
inline void
vcat(const char* s, [[maybe_unused]] T&& v)
{
  printf("%s: rvalue\n", s);
}

} // end namespace tcii::tests

void refTest()
{
  puts("\n**LVALUE/RVALUE REFERENCE TUTORIAL");
  puts("**DECLARATIONS");

  using namespace tcii::tests;

  const X& r2 = X{"C++"}; // (2)
  vcat("r2", r2);
  X&& r4 = X{"C++"}; // (4)
  vcat("r4", r4);

  r4[1] = r4[2] = '9';
  r2.print("r2");
  r4.print("r4");

  X x1 = X("C++"); // (6)
  vcat("x1", x1);
  X x2 = "C99"; // (7)
  vcat("x2", x2);

  auto&& r7 = x1; // OK (9)
  vcat("r7", r7);
  X& r8 = r7; // (10)
  vcat("r8", r8);
  auto x3 = r8; // (11)
  vcat("x3", x3);
  X&& r9 = static_cast<X&&>(r8); // (12)
  vcat("r9", r9);
  auto x4 = std::move(r8); // (13)
  vcat("x4", x4);

  puts("**ASSIGNMENTS");
  r9 = r4; // (14)
  vcat("r9", r9);
  r9.print("r9");
  r8 = r4; // (15)
  vcat("r8", r8);
  r8.print("r8");
  r4 = x2; // (16)
  vcat("r4", r4);
  r4.print("r4");
  r8 = x1; // (17)
  vcat("r8", r8);
  r8.print("r8");
  r9 = std::move(r4); // (18)
  vcat("r9", r9);
  r9.print("r9");
  r7 = r9; // (19)
  vcat("r7", r7);
  r7.print("r7");
  r8 = std::move(r7); // (20)
  vcat("r8", r8);
  r8.print("r8");
  r9 = r2 + X{"20"}; // (21)
  vcat("r9", r9);
  r9.print("r9");
  if constexpr (false)
  {
    puts("\n**MORE");
    X s1{"s1"};
    vcat("s1", s1);
    //X&& x = s1;
    //vcat("x", x);
    const X s2{"s2"};
    vcat("s2", s2);
    vcat("X{\"temp\"}", X{"temp"});
    vcat("s1 + s2", s1 + s2);
    //auto& r1 = s1 + s2;
    auto& r1 = s1;
    vcat("r1", r1);
    auto& r2 = s2;
    vcat("r2", r2);
    auto&& temp = X{"temp"};
    vcat("temp", temp);
    auto&& s1s2 = s1 + s2;
    vcat("s1s2", s1s2);
    temp[0] = 'x';
    s1s2[0] = 'x';
    printf("**\n%s\n%s\n", temp.c_str(), s1s2.c_str());
    temp = r1;
    s1s2 = temp + r2;
    s1 = s1s2;
    printf("**\n%s\n%s\n", temp.c_str(), s1s2.c_str());
  }
}

/*
Exercise: what happens if the class tcii::tests::X does not declare
copy and move constructors and/or copy and move operators?
*/

#endif // __RefTest_h
