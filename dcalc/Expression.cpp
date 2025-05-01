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
  Expression add(const Expression&) const final;
  Expression mul(const Expression&) const final;
  Expression composite(const Expression&) const final;
  Expression diff() const final;
  std::string toString() const final;

}; // NullExpression

float
NullExpression::eval(float) const
{
  throw badInvocation("NullExpression", __func__);
}

Expression
NullExpression::add(const Expression&) const
{
  throw badInvocation("NullExpression", __func__);
}

Expression
NullExpression::mul(const Expression&) const
{
  throw badInvocation("NullExpression", __func__);
}

Expression
NullExpression::composite(const Expression&) const
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
// BinaryExpression class
// ================
struct BinaryExpression: Expression::Body
{
  const Expression e1;
  const Expression e2;
  const char op;

  BinaryExpression(const Expression& e1, const Expression e2, char op):
    e1{e1},
    e2{e2},
    op{op}
  {
    // do nothing
  }

  std::string toString() const override;

}; // BinaryExpression

std::string
BinaryExpression::toString() const
{
  return e1.toString() + op + e2.toString();
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
  Expression add(const Expression&) const override;
  Expression mul(const Expression&) const override;
  Expression composite(const Expression&) const override;
  Expression diff() const override;

}; // AddExpression

float
AddExpression::eval(float x) const
{
  return e1.eval(x) + e2.eval(x);
}

Expression
AddExpression::add(const Expression& e) const
{
  if (auto a = as<AddExpression>(e))
  {
    Expression r = *this;
    return r + a->e1 + a->e2;
  }

  auto r1 = body(e1)->add(e);
  auto n1 = r1.isNull();
  auto r2 = e2;
  auto n2 = true;

  if (n1)
  {
    r2 = body(e2)->add(e);
    n2 = r2.isNull();
    r1 = e1;
  }
  return n1 && n2 ? Expression{} : r1 + r2;
}

Expression
AddExpression::mul(const Expression& e) const
{
  if (auto a = as<AddExpression>(e))
  {
    Expression r = *this;
    return r * a->e1 + r * a->e2;
  }

  auto r1 = body(e1)->mul(e);
  auto r2 = body(e2)->mul(e);

  return r1 + r2;
}

Expression
AddExpression::composite(const Expression& e) const
{
  return e1.composite(e) + e2.composite(e);
}

Expression
AddExpression::diff() const
{
  return e1.diff() + e2.diff();
}


//////////////////////////////////////////////////////////
//
// MulExpression class
// =============
struct MulExpression: BinaryExpression
{
  MulExpression(const Expression& e1, const Expression& e2):
    BinaryExpression{e1, e2, '*'}
  {
    // do nothing
  }

  float eval(float) const override;
  Expression mul(const Expression&) const override;
  Expression composite(const Expression&) const override;
  Expression diff() const override;

}; // MulExpression

float
MulExpression::eval(float x) const
{
  return e1.eval(x) * e2.eval(x);
}

Expression
MulExpression::mul(const Expression& e) const
{
  if (auto a = as<MulExpression>(e))
  {
    Expression r = *this;
    return r * a->e1 * a->e2;
  }

  auto r1 = body(e1)->mul(e);
  auto n1 = r1.isNull();
  auto r2 = e2;
  auto n2 = true;

  if (n1)
  {
    r2 = body(e2)->mul(e);
    n2 = r2.isNull();
    r1 = e1;
  }
  return n1 && n2 ? Expression{} : r1 * r2;
}

Expression
MulExpression::composite(const Expression& e) const
{
  return e1.composite(e) * e2.composite(e);
}

Expression
MulExpression::diff() const
{
  return e1.diff() * e2 + e1 * e2.diff();
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
  Expression add(const Expression&) const final;
  Expression mul(const Expression&) const final;
  Expression composite(const Expression&) const final;
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
Term::add(const Expression& e) const
{
  if (auto t = as<Term>(e))
    if (n == t->n)
      return Expression::term(n, a + t->a);
  if (auto a = as<AddExpression>(e))
    return a->add(*this);
  return Expression{};
}

Expression
Term::mul(const Expression& e) const
{
  if (auto c = as<Constant>(e))
    return Expression::term(n, c->value * a);
  if (auto t = as<Term>(e))
    return Expression::term(n + t->n, a * t->a);
  if (auto a = as<AddExpression>(e))
    return a->mul(*this);
  if (auto m = as<MulExpression>(e))
    return m->mul(*this);
  return Expression{};
}

Expression
Term::composite(const Expression& e) const
{
  if (auto c = as<Constant>(e))
    return Term::eval(c->value);
  if (auto t = as<Term>(e))
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
// Constant implementation
// ========
float
Constant::eval(float) const
{
  return value;
}

Expression
Constant::add(const Expression& e) const
{
  if (auto c = as<Constant>(e))
    return value + c->value;
  if (auto a = as<AddExpression>(e))
    return a->add(*this);
  return Expression{};
}

Expression
Constant::mul(const Expression& e) const
{
  if (auto c = as<Constant>(e))
    return value * c->value;
  if (auto t = as<Term>(e))
    return Expression::term(t->n, value * t->a);
  if (auto a = as<AddExpression>(e))
    return a->mul(*this);
  if (auto m = as<MulExpression>(e))
    return m->mul(*this);
  return Expression{};
}

Expression
Constant::composite(const Expression&) const
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
// Expression::Body implementation
// ================
Expression
Expression::Body::add(const Expression&) const
{
  return Expression{};
}

Expression
Expression::Body::mul(const Expression&) const
{
  return Expression{};
}


//////////////////////////////////////////////////////////
//
// Expression implementation
// ==========
Expression
Expression::operator +(const Expression& e) const
{
  if (auto r = _body->add(e); !r.isNull())
    return r;
  if (auto e1 = Body::as<Constant>(*this))
  {
    if (e1->value == 0)
      return e;
  }
  else if (auto e2 = Body::as<Constant>(e))
  {
    if (e2->value == 0)
      return *this;
  }
  return *new AddExpression{*this, e};
}

Expression
Expression::operator -(const Expression& e) const
{
  // TODO
  return Expression{};
}

Expression
Expression::operator *(const Expression& e) const
{
  if (auto r = _body->mul(e); !r.isNull())
    return r;
  if (auto e1 = Body::as<Constant>(*this))
  {
    if (e1->value == 0)
      return 0;
    if (e1->value == 1)
      return e;
  }
  else if (auto e2 = Body::as<Constant>(e))
  {
    if (e2->value == 0)
      return 0;
    if (e2->value == 1)
      return *this;
  }
  return *new MulExpression{*this, e};
}

Expression
Expression::operator /(const Expression& e) const
{
  // TODO
  return Expression{};
}

Expression
Expression::operator -() const
{
  // TODO
  return Expression{};
}

Expression
Expression::pow(const Expression& e) const
{
  // TODO
  return Expression{};
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
