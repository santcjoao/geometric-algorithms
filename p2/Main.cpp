#include "TriangleMesh.h"

int
main()
{
  using namespace tcii::cg;

  auto filename = "../../meshes/f-16.obj";
  auto mesh = readOBJ(filename);

  if (!mesh)
    printf("Could not read '%s'\n", filename);
  else
    mesh->print(filename);
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
