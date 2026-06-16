#include "Renderer.h"
#include "RigidBody.h"
#include "Scene.h"
#include "SphereShape.h"

int main() {
  using namespace tcii::physx;

  auto scene = Scene::New();
  auto body1 = RigidBody::New();
  auto body2 = RigidBody::New();

  body1->addShape(new SphereShape{{1, 2, 3}, 1});
  body1->addShape(new SphereShape{{4, 5, 6}, 2});
  body2->addShape(new SphereShape{{7, 8, 9}, 3});
  scene->addActor(body1);
  scene->addActor(body2);

  Renderer renderer;

  scene->render(renderer);
  destroy(scene);
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}