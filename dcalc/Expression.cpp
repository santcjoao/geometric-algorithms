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
// Term class
// ====
class Term final: public Expression::Body
{
public:
  const float n; // exponent
  const float a; // coefficient

  static Expression New(float n, float a)
  {
    return n == 0 || a == 0 ? a : Expression{*new Term{n, a}};
  }

  float eval(float) const final;
  Expression add(const Expression&) const final;
  Expression mul(const Expression&) const final;
  Expression composite(const Expression&) const final;
  bool equals(const Expression&) const final;
  Expression diff() const final;
  std::string toString() const final;

private:
  Term(float n, float a):
    n{n},
    a{a}
  {
    assert(n != 0);
    assert(a != 0);
  }

}; // Term


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
  bool equals(const Expression&) const override;
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
  return e1 * e + e2 * e;
}

Expression
AddExpression::composite(const Expression& e) const
{
  return e1.composite(e) + e2.composite(e);
}

bool
AddExpression::equals(const Expression& e) const
{
  if (auto a = Body::as<AddExpression>(e))
    return e1 == a->e1 && e2 == a->e2 || e1 == a->e2 && e2 == a->e1;
  return false;
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
  bool equals(const Expression&) const override;
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

bool
MulExpression::equals(const Expression& e) const
{
  if (auto a = Body::as<MulExpression>(e))
    return e1 == a->e1 && e2 == a->e2 || e1 == a->e2 && e2 == a->e1;
  return false;
}

Expression
MulExpression::diff() const
{
  return e1.diff() * e2 + e1 * e2.diff();
}


//////////////////////////////////////////////////////////
//
// NativeFunction class
// ==============
class NativeFunction
{
public:
  virtual float eval(float) const  = 0;
  virtual Expression diff(const Expression&) const  = 0;
  virtual std::string toString() const = 0;

  static const NativeFunction& exp();

private:
  class Exp;

}; // NativeFunction


//////////////////////////////////////////////////////////
//
// Function class
// ========
class Function final: public Expression::Body
{
public:
  const float a; // coefficient

  float eval(float) const final;
  Expression add(const Expression&) const final;
  Expression mul(const Expression&) const final;
  Expression composite(const Expression&) const final;
  bool equals(const Expression&) const final;
  Expression diff() const final;
  std::string toString() const final;

  static Expression New(const NativeFunction&, const Expression&);
  static Expression New(const Function&, float = 1);

private:
  const NativeFunction& function;
  Expression argument;

  Function(const NativeFunction& f, const Expression& a, float c):
    function{f},
    argument{a},
    a{c}
  {
    assert(a != 0);
  }

  auto sameBase(const Function* f) const
  {
    return &function == &f->function && argument == f->argument;
  }

}; // Function

Expression
Function::New(const NativeFunction& f, const Expression& a)
{
  if (auto c = as<Constant>(a))
    return f.eval(c->value);
  return *new Function{f, a, 1};
}

Expression
Function::New(const Function& f, float c)
{
  return c != 0 ? *new Function{f.function, f.argument, c} : Expression{0};
}

float
Function::eval(float x) const
{
  return function.eval(argument.eval(x)) * a;
}

Expression
Function::add(const Expression& e) const
{
  if (auto a = as<AddExpression>(e))
    return a->add(*this);
  if (auto f = as<Function>(e); f && sameBase(f))
    return New(*this, a + f->a);
  return Expression{};
}

Expression
Function::mul(const Expression& e) const
{
  if (auto c = as<Constant>(e))
    return New(*this, c->value * a);
  if (auto a = as<AddExpression>(e))
    return a->mul(*this);
  if (auto m = as<MulExpression>(e))
    return m->mul(*this);
  if (auto f = as<Function>(e); f && sameBase(f))
  {
    // TODO
  }
  return Expression{};
}

Expression
Function::composite(const Expression& e) const
{
  return New(function, argument.composite(e)) * a;
}

bool
Function::equals(const Expression& e) const
{
  if (auto f = as<Function>(e))
    return a == f->a && sameBase(f);
  return false;
}

Expression
Function::diff() const
{
  auto da = argument.diff();

  if (auto c = as<Constant>(da))
    if (c->value == 0)
      return 0;
  return function.diff(argument) * da * a;
}

std::string
Function::toString() const
{
  auto s = std::format("{}({})", function.toString(), argument.toString());

  return a == 1 ? s : a == -1 ? std::format("-{}", s) :
    format("{}*{}", a, s);
}


//////////////////////////////////////////////////////////
//
// NativeFunction::Exp class
// ===================
class NativeFunction::Exp final: public NativeFunction
{
  float eval(float) const final;
  Expression diff(const Expression&) const final;
  std::string toString() const final;

}; // NativeFunction::Exp

float
NativeFunction::Exp::eval(float x) const
{
  return std::exp(x);
}

Expression
NativeFunction::Exp::diff(const Expression& a) const
{
  return Function::New(exp(), a);
}

std::string
NativeFunction::Exp::toString() const
{
  return "exp";
}

const NativeFunction&
NativeFunction::exp()
{
  static const Exp exp;
  return exp;
}


//////////////////////////////////////////////////////////
//
// Term implementation
// ====
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
      return New(n, a + t->a);
  if (auto a = as<AddExpression>(e))
    return a->add(*this);
  return Expression{};
}

Expression
Term::mul(const Expression& e) const
{
  if (auto c = as<Constant>(e))
    return New(n, c->value * a);
  if (auto t = as<Term>(e))
    return New(n + t->n, a * t->a);
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
    return New(n * t->n, a * t->a);
  return e.pow(n) * a;
}

bool
Term::equals(const Expression& e) const
{
  if (auto t = Body::as<Term>(e))
    return n == t->n && a == t->a;
  return false;
}

Expression
Term::diff() const
{
  return n == 1 ? a : New(n - 1, a * n);
}

std::string
Term::toString() const
{
  auto a1 = a == 1;

  if (n == 1)
    return a1 ? "x" : a == -1 ? "-x" :
      std::format("{}x", a);

  auto s = std::format("x^{}", n);

  return a1 ? s : a == -1 ? std::format("-{}", s) :
    std::format("{}{}", a, s);
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
    return Term::New(t->n, value * t->a);
  if (auto a = as<AddExpression>(e))
    return a->mul(*this);
  if (auto m = as<MulExpression>(e))
    return m->mul(*this);
  if (auto f = as<Function>(e))
    return Function::New(*f, value * f->a);
  return Expression{};
}

Expression
Constant::composite(const Expression&) const
{
  return *this;
}

bool
Constant::equals(const Expression& e) const
{
  if (auto c = Body::as<Constant>(e))
    return value == c->value;
  return false;
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

bool
Expression::Body::equals(const Expression& e) const
{
  return this == body(e);

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
  return {};
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
  return {};
}

Expression
Expression::operator -() const
{
  // TODO
  return {};
}

Expression
Expression::pow(const Expression& e) const
{
  // TODO
  return {};
}

const Expression::Body&
Expression::null()
{
  static const NullExpression null;
  return null;
}

Expression
Expression::exp(const Expression& argument)
{
  return Function::New(NativeFunction::exp(), argument);
}

Expression
x(float exponent, float coefficient)
{
  return Term::New(exponent, coefficient);
}

} // end namespace tcii::dcalc
