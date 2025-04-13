#include "Renderer.h"
#include "SphereShape.h"

namespace tcii::physx
{ // begin namespace tcii::physx

Bounds3f
SphereShape::bounds() const
{
  Bounds3f bounds;

  bounds.inflate(center - Vec3f{_radius, _radius, _radius});
  bounds.inflate(center + Vec3f{_radius, _radius, _radius});
  return bounds;
}
  
void
SphereShape::render(Renderer& renderer) const
{
  std::cout << "**Sphere" << '\n';
  renderer.draw("center", center).draw("radius", _radius);
}

} // end namespace tcii::physx
