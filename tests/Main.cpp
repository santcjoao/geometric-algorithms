#include "FuncTest.h"
#include "InitTest.h"
#include "ListTest.h"
#include "TypeTest.h"
#include <cstdio>

int
main()
{
  typeTest();
  funcTest();
  initTest();
  listTest();
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
