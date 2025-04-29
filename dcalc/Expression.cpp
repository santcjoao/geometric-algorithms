#include "util/Exception.h"
#include "Expression.h"
#include <cassert>
#include <format>

namespace tcii::dcalc
{ // begin namesapce tcii::dcalc


//////////////////////////////////////////////////////////
//
// NullExpression class
// ==============
struct NullExpression final: Expression::Body
{
  NullExpression()
  {
    makeUse(this);
  }

  float eval(float) const final;
  Expression composedWith(const Expression&) const final;
  Expression diff() const final;
  std::string toString() const final;

}; // NullExpression

float
NullExpression::eval(float) const
{
  throw badInvocation("NullExpression", __func__);
}

Expression
NullExpression::composedWith(const Expression&) const
{
  throw badInvocation("NullExpression", __func__);
}

Expression
NullExpression::diff() const
{
  throw badInvocation("NullExpression", __func__);
}

std::string
NullExpression::toString() const
{
  return "<null>";
}


//////////////////////////////////////////////////////////
//
// Constant implementation
// ========
float
Constant::eval(float) const
{
  return value;
}

Expression
Constant::composedWith(const Expression&) const
{
  return *this;
}

Expression
Constant::diff() const
{
  return 0;
}

std::string
Constant::toString() const
{
  return std::format("{}", value);
}


//////////////////////////////////////////////////////////
//
// Term class
// ====
class Term final: public Expression::Body
{
public:
  const float n; // exponent
  const float a; // coefficient

  Term(float n, float a):
    n{n},
    a{a}
  {
    assert(n != 0);
    assert(a != 0);
  }

  float eval(float) const final;
  Expression composedWith(const Expression&) const final;
  Expression diff() const final;
  std::string toString() const final;

}; // Term

float
Term::eval(float x) const
{
  if (n != 1)
    x = std::pow(x, n);
  return a != 1 ? x : a * x;
}

Expression
Term::composedWith(const Expression& e) const
{
  if (auto c = e.as<Constant>())
    return Term::eval(c->value);
  if (auto t = e.as<Term>())
    return Expression::term(n * t->n, a * t->a);
  return e.pow(n) * a;
}

Expression
Term::diff() const
{
  return n == 1 ? a : Expression::term(n - 1, a * n);
}

std::string
Term::toString() const
{
  auto a1 = a == 1;
  auto n1 = n == 1;

  return a1 && n1 ? "x" :
    a1 ? std::format("x^{}", n) : n1 ? std::format("{}x", a) :
    std::format("{}x^{}", a, n);
}


//////////////////////////////////////////////////////////
//
// BinaryExpression class
// ================
struct BinaryExpression: Expression::Body
{
  Expression _e1;
  Expression _e2;
  char _op;

  BinaryExpression(const Expression& e1, const Expression e2, char op):
    _e1{e1},
    _e2{e2},
    _op{op}
  {
    // do nothing
  }

  std::string toString() const override;

}; // BinaryExpression

std::string
BinaryExpression::toString() const
{
  return _e1.toString() + _op + _e2.toString();
}


//////////////////////////////////////////////////////////
//
// AddExpression class
// =============
struct AddExpression: BinaryExpression
{
  AddExpression(const Expression& e1, const Expression& e2):
    BinaryExpression{e1, e2, '+'}
  {
    // do nothing
  }

  float eval(float) const override;
  Expression composedWith(const Expression&) const override;
  Expression diff() const override;

}; // AddExpression

float
AddExpression::eval(float x) const
{
  return _e1.eval(x) + _e2.eval(x);
}

Expression
AddExpression::composedWith(const Expression& e) const
{
  return _e1.composedWith(e) + _e2.composedWith(e);
}

Expression
AddExpression::diff() const
{
  return _e1.diff() + _e2.diff();
}


//////////////////////////////////////////////////////////
//
// Expression implementation
// ==========
Expression
Expression::operator +(const Expression& e) const
{
  if (auto e1 = as<Constant>())
  {
    if (e1->value == 0)
      return e;
    if (auto e2 = e.as<Constant>())
      return e1->value + e2->value;
  }
  else if (auto e2 = e.as<Constant>())
  {
    if (e2->value == 0)
      return *this;
  }
  else if (auto e1 = as<Term>())
    if (auto e2 = e.as<Term>())
      if (e1->n == e2->n)
      {
        if (auto a = e1->a + e2->a)
          return term(e1->n, a);
        return 0;
      }
  return *new AddExpression{*this, e};
}

Expression
Expression::operator -(const Expression& e) const
{
  // TODO
  return null();
}

Expression
Expression::operator *(const Expression& e) const
{
  // TODO
  return null();
}

Expression
Expression::operator /(const Expression& e) const
{
  // TODO
  return null();
}

Expression
Expression::operator -() const
{
  // TODO
  return null();
}

Expression
Expression::pow(const Expression& e) const
{
  // TODO
  return null();
}

Expression
Expression::term(float n, float a)
{
  return n == 0 || a == 0 ? a : Expression{*new Term{n, a}};
}


const Expression::Body&
Expression::null()
{
  static const NullExpression null;
  return null;
}

} // end namespace tcii::dcalc
