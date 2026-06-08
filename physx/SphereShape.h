#ifndef __SphereShape_h
#define __SphereShape_h

#include "Shape.h"

namespace tcii::physx
{ // begin namespace tcii::physx

using namespace cg;

class SphereShape: public Shape
{
public:
  Vec3f center;

  SphereShape(const Vec3f& center, float radius):
    center{center}
  {
    setRadius(radius);
  }

  void setRadius(float radius)
  {
    _radius = radius < 0 ? 0 : radius;
  }

  auto radius() const
  {
    return _radius;
  }

  virtual Bounds3f bounds() const final;
  virtual void render(Renderer&) const final;

private:
  float _radius;

}; // SphereShape

} // end namespace tcii::physx

#endif // __SphereShape_h
