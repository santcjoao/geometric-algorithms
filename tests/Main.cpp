#include "FuncTest.h"
#include "InitTest.h"
#include "TypeTest.h"
#include <cstdio>

int
main()
{
  typeTest();
  funcTest();
  initTest();
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
