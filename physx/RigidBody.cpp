#include "RigidBody.h"

namespace tcii::physx
{ // begin namespace tcii::physx

RigidBody*
RigidBody::New()
{
  static int nextId;
  auto body = new RigidBody{};

  body->setName("RB %d", ++nextId);
  return body;
}

Bounds3f
RigidBody::bounds() const
{
  Bounds3f bounds;

  for (auto& shape : _shapes)
    bounds.inflate(shape.bounds());
  return bounds;
}

void
RigidBody::render(Renderer& renderer) const
{
  std::cout << "**" << name() << '\n';
  std::cout << "Bounds " << bounds() << '\n';
  for (auto& shape : _shapes)
    shape.render(renderer);
}

} // end namespace tcii::physx
