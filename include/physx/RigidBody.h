#ifndef __RigidBody_h
#define __RigidBody_h

#include "Shape.h"

namespace tcii::physx { // begin namespace tcii::physx

using namespace cg;

class RigidBody : public Actor {
public:
  static RigidBody *New();

  void addShape(Shape *shape);
  void removeShape(Shape *shape);

  void removeAllShapes() { _shapes.clear(); }

  auto &shapes() const { return _shapes; }

  auto shapeCount() const { return _shapes.size(); }

  Bounds3f bounds() const override;
  void render(Renderer &) const override;

private:
  ObjectList<Shape> _shapes;

}; // RigidBody

inline void RigidBody::addShape(Shape *shape) {
  if (_shapes.add(shape))
    shape->_actor = this;
}

inline void RigidBody::removeShape(Shape *shape) {
  if (shape->_actor == this) {
    _shapes.remove(shape);
    destroy(shape);
  }
}

} // end namespace tcii::physx

#endif // __RigidBody_h
