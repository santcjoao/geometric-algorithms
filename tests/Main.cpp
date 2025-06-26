#include "DflOpTest.h"
#include "FuncTest.h"
#include "InitTest.h"
#include "lambdaTest.h"
#include "ListTest.h"
#include "MatrixTest.h"
#include "RefTest.h"
#include "TypeTest.h"
#include "VariadicTest.h"

int
main()
{
  using TestFunc = std::function<void()>;

  const struct
  {
    const char* title;
    int cmd;
    TestFunc func;

  } tests[]
  {
    {"Numeric/struct/pointer/array/function type", '1', typeTest},
    {"Function templates/constexpr function", '2', funcTest},
    {"Object initialization", '3', initTest},
    {"Class templates/int list iteration", '4', listTest},
    {"Lvalue/Rvalue reference tutorial", '5', refTest},
    {"Special member functions", '6', dflOpTest},
    {
      "Exceptions/Copy&Move/Smart pointers", '7', []()
        {
          puts("\n**EXCEPTIONS/COPY&MOVE/SMART POINTERS");
          matrixTest();
          vectorTest();
        }
    },
    {"Variadic templates", '8', variadicTest},
    {"Functor/Lambda/Fold expression", '9', lambdaTest},
  };

  for (bool found;;)
  {
    puts("**C++ feature tests (SEE THE CODE):");
    for (const auto& test : tests)
      printf("[%c] %s\n", test.cmd, test.title);
    puts("[x] Exit\n**Enter an option<x>: ");

    char input[3];

    fgets(input, sizeof input, stdin);

    auto option = *input;

    if (option == '\n')
      break;
    found = false;
    if (input[1] != '\n')
      while (getchar() != '\n')
        ;
    else
    {
      if (option == 'x')
        break;
      for (const auto& test : tests)
        if (option == test.cmd)
        {
          found = true;
          if (test.func == nullptr)
            puts("**Test not implemented yet");
          else
            try
            {
              test.func();
            }
            catch (const std::exception& e)
            {
              printf("**Error: %s\n", e.what());
            }
        }
    }
    if (!found)
      puts("**Invalid option. Try again");
    putchar('\n');
  }
  return 0;
}
