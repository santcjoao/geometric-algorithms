#ifndef __ListTest_h
#define __ListTest_h

#include "util/LinkedList.h"

namespace tcii::tests
{ // begin namespace tcii::tests

inline void
printInt(int i)
{
  printf("%d\n", i);
}

inline void
intListTest()
{
  puts("\n**INT LIST ITERATION");

  cg::LinkedList<int> list;
  int sum{};

  for (int i = 0; i < 10; ++i)
    list.add(i);
  list.iterate([&sum](auto i)
    {
      sum += i;
    }
  );
  printf("Sum: %d\n", sum);
  for (auto end = list.end(), lit = list.begin(); lit != end; ++lit)
    printInt(*lit);
  for (auto i : list)
    printInt(i);
  cg::printUsedMemory();
  cg::printMemoryMap();
  list.clear();
  cg::printUsedMemory();
}

} // end namespace tcii::tests

inline void
listTest()
{
  using namespace tcii::tests;

  intListTest();
}

#endif // __ListTest_h
