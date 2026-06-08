#ifndef __Scene_h
#define __Scene_h

#include "Actor.h"

namespace tcii::physx
{ // begin namespace tcii::physx

class Scene: public NameableObject
{
public:
  static Scene* New();

  void addActor(Actor* actor);
  void removeActor(Actor* actor);

  void removeAllActors()
  {
    _actors.clear();
  }

  auto& actors() const
  {
    return _actors;
  }

  auto actorCount() const
  {
    return _actors.size();
  }

  Bounds3f bounds() const;
  void render(Renderer&) const;

private:
  ObjectList<Actor> _actors;

}; // Scene

inline void
Scene::addActor(Actor* actor)
{
  if (_actors.add(actor))
    actor->_scene = this;
}

inline void
Scene::removeActor(Actor* actor)
{
  if (actor->_scene == this)
  {
    _actors.remove(actor);
    destroy(actor);
  }
}

} // end namespace tcii::physx

#endif // __Scene_h
