#include "FuncTest.h"
#include "TypeTest.h"
#include <cstdio>

int
main()
{
  typeTest();
  funcTest();
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
