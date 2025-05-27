#include "ParticleSystem.h"
#include <cstdio>
#include <iostream>

int
main()
{
  using namespace tcii::physx;

  auto ps = ParticleSystem<>::New();

  ps->setParticleBuffer(1000);

  auto pb = ps->particles();
  auto pc = pb->particleCount();

  printf("%s (capacity: %d)\n", ps->name(), pb->capacity());
  std::cout << "Particles: " << pc << "\nBounds: ";
  (pc ? std::cout << ps->bounds() : std::cout << "<empty>") << '\n';
  puts("Press any key to exit...");
  (void)getchar();
  destroy(ps);
  return 0;
}
