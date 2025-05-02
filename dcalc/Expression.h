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

  Expression& operator +=(const Expression&);
  Expression& operator -=(const Expression&);
  Expression& operator *=(const Expression&);
  Expression& operator /=(const Expression&);

  bool operator !=(const Expression&) const;
  bool operator ==(const Expression&) const;

  float eval(float) const;
  Expression composite(const Expression&) const;
  Expression diff() const;
  std::string toString() const;
  bool isNull() const;

  static Expression term(float, float);
  static Expression exp(const Expression&);

private:
  ObjectPtr<Body> _body{&null()};

  explicit Expression(const Body& body):
    _body{&body}
  {
    // do nothing
  }

  static const Body& null();

  friend class Constant;
  friend class Term;

}; // Expression

Expression x(float exponent = 1, float coefficient = 1);


//////////////////////////////////////////////////////////
//
// Expression::Body class
// ================
class Expression::Body: public SharedObject
{
public:
  virtual ~Body() = default;

  virtual float eval(float) const = 0;
  virtual Expression add(const Expression&) const;
  virtual Expression mul(const Expression&) const;
  virtual Expression composite(const Expression&) const = 0;
  virtual bool equals(const Expression&) const;
  virtual Expression diff() const = 0;
  virtual std::string toString() const = 0;

  operator Expression() const
  {
    return Expression{*this};
  }

  static auto body(const Expression& e)
  {
    return e._body.get();
  }

  template <typename E> static auto as(const Expression& e)
  {
    return dynamic_cast<E*>(body(e));
  }

}; // Expression::Body


//////////////////////////////////////////////////////////
//
// Constant class
// ========
class Constant final: public Expression::Body
{
public:
  const float value;

  Constant(float value):
    value(value)
  {
    // do nothing
  }

  float eval(float) const final;
  Expression add(const Expression&) const final;
  Expression mul(const Expression&) const final;
  Expression composite(const Expression&) const final;
  bool equals(const Expression&) const final;
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
Expression::composite(const Expression& e) const
{
  return _body->composite(e);
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

inline bool
Expression::operator ==(const Expression& e) const
{
  return _body->equals(e);
}

inline bool
Expression::operator !=(const Expression& e) const
{
  return !operator ==(e);
}

inline bool
Expression::isNull() const
{
  return _body == &null();
}

} // end namespace tcii::dcalc

inline std::ostream&
operator <<(std::ostream& os, const tcii::dcalc::Expression& e)
{
  os << e.toString();
  return os;
}

#endif // __Expression_h
