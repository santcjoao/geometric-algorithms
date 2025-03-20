#include <iostream>

int
main(int argc, char* argv[])
{
  using namespace std;

  cout << "Welcome to modern C++\n";
  cout << "Command line:\n";
  for (int i = 0; i < argc;)
    cout << argv[i++] << '\n';
  return 0;
}
