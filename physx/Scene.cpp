#include "Scene.h"

namespace tcii::physx
{ // begin namespace tcii::physx

Scene*
Scene::New()
{
  static int nextId;
  auto scene = new Scene{};

  scene->setName("Scene %d", ++nextId);
  return scene;
}

Bounds3f
Scene::bounds() const
{
  Bounds3f bounds;

  for (auto& actor : _actors)
    bounds.inflate(actor.bounds());
  return bounds;
}

void
Scene::render(Renderer& renderer) const
{
  std::cout << "**" << name() << '\n';
  std::cout << "Bounds " << bounds() << '\n';
  for (auto& actor : _actors)
    actor.render(renderer);
}

} // end namespace tcii::physx
