#ifndef __Expression_h
#define __Expression_h

#include "util/SharedObject.h"
#include <iostream>

namespace tcii::dcalc
{ // begin namesapce tcii::dcalc

using namespace cg;



//////////////////////////////////////////////////////////
//
// Expression class
// ==========
class Expression
{
public:
  class Body;

  Expression() = default;
  Expression(float);

  Expression operator +(const Expression&) const;
  Expression operator -(const Expression&) const;
  Expression operator *(const Expression&) const;
  Expression operator /(const Expression&) const;
  Expression operator -() const;
  Expression operator +() const;
  Expression pow(const Expression&) const;

  static Expression term(float = 1, float = 1);

  Expression& operator +=(const Expression&);
  Expression& operator -=(const Expression&);
  Expression& operator *=(const Expression&);
  Expression& operator /=(const Expression&);

  float eval(float) const;
  Expression composedWith(const Expression&) const;
  Expression diff() const;
  std::string toString() const;

private:
  ObjectPtr<Body> _body{&null()};

  explicit Expression(const Body& body):
    _body{&body}
  {
    // do nothing
  }

  template <typename E> auto as() const
  {
    return dynamic_cast<E*>(_body.get());
  }

  static const Body& null();

  friend class Term;

}; // Expression

inline auto
x()
{
  return Expression::term();
}


//////////////////////////////////////////////////////////
//
// Expression::Body class
// ================
class Expression::Body: public SharedObject
{
public:
  virtual ~Body() = default;

  virtual float eval(float) const = 0;
  virtual Expression composedWith(const Expression&) const = 0;
  virtual Expression diff() const = 0;
  virtual std::string toString() const = 0;

  operator Expression() const
  {
    return Expression{*this};
  }

}; // Expression::Body


//////////////////////////////////////////////////////////
//
// Constant class
// ========
struct Constant final: Expression::Body
{
  const float value;

  Constant(float value):
    value(value)
  {
    // do nothing
  }

  float eval(float) const final;
  Expression composedWith(const Expression&) const final;
  Expression diff() const final;
  std::string toString() const final;

}; // Constant


//////////////////////////////////////////////////////////
//
// Expression inline implementation
// ==========
inline Expression::Expression(float value):
  _body{new Constant{value}}
{
  // do nothing
}

inline float
Expression::eval(float x) const
{
  return _body->eval(x);
}

inline Expression
Expression::composedWith(const Expression& e) const
{
  return _body->composedWith(e);
}

inline Expression
Expression::diff() const
{
  return _body->diff();
}

inline std::string
Expression::toString() const
{
  return _body->toString();
}

inline Expression
Expression::operator +() const
{
  return *this;
}

inline Expression&
Expression::operator +=(const Expression& e)
{
  return *this = *this + e;
}

inline Expression&
Expression::operator -=(const Expression& e)
{
  return *this = *this - e;
}

inline Expression&
Expression::operator *=(const Expression& e)
{
  return *this = *this * e;
}

inline Expression&
Expression::operator /=(const Expression& e)
{
  return *this = *this / e;
}

} // end namespace tcii::dcalc

inline std::ostream&
operator <<(std::ostream& os, const tcii::dcalc::Expression& e)
{
  os << e.toString();
  return os;
}

#endif // __Expression_h
