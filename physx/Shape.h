#ifndef __Shape_h
#define __Shape_h

#include "Actor.h"

namespace tcii::physx
{ // begin namespace tcii::physx

using namespace cg;

class RigidBody;

class Shape: ObjectListNode<Shape>
{
public:
  virtual ~Shape() = default;

  auto actor() const
  {
    return _actor;
  }

  template <typename T> bool is() const
  {
    return dynamic_cast<const T*>(this);
  }

  virtual Bounds3f bounds() const = 0;
  virtual void render(Renderer&) const = 0;

private:
  Actor* _actor{};

  friend RigidBody;

}; // Shape

} // end namespace tcii::physx

#endif // __Shape_h
