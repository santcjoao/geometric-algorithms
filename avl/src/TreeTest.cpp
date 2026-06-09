#include "../include/AVL.h"
#include <cstdio>
#include <cstring>
#include <string>

using namespace tcii;

template <typename T> struct Vec3;
class Traced;

// Helpers de impressao
void printInt(const int &i) { printf("%d ", i); }
void printVec3(const Vec3<float> &v);
void printTraced(const Traced &t);

// Funcao generica de teste
template <typename T, typename C, typename PrintFn>
void runFullTest(const char *title, std::initializer_list<T> values,
                 const T &findVal, const T &eraseVal, PrintFn print) {
  printf("--- %s ---\n", title);

  avl::Tree<T, C> t;

  // insert
  t.insert(values);
  printf("insert(%zu):   size=%d  height=%d  empty=%s\n", values.size(),
         t.size(), t.height(), t.empty() ? "true" : "false");

  // iterate
  printf("iterate:     ");
  t.iterate(print);
  puts("");

  // range-for
  printf("range-for:   ");
  for (const auto &v : t)
    print(v);
  puts("");

  // reverse iterator
  printf("reverse:     ");
  for (auto it = t.rbegin(); it != t.rend(); ++it)
    print(*it);
  puts("");

  // find e contains
  printf("find:        %s\n",
         t.find(findVal) != t.end() ? "found" : "not found");
  printf("contains:    %s\n", t.contains(findVal) ? "true" : "false");

  // erase
  bool erased = t.erase(eraseVal);
  printf("erase:       %s  ->  size=%d  height=%d\n",
         erased ? "ok" : "not found", t.size(), t.height());
  printf("after erase: ");
  t.iterate(print);
  puts("");

  // clear
  t.clear();
  printf("clear:       size=%d  empty=%s\n\n", t.size(),
         t.empty() ? "true" : "false");
}

// Agregado: Vec3
template <typename T> struct Vec3 {
  T x, y, z;
};

template <typename T> struct Vec3Less {
  bool operator()(const Vec3<T> &a, const Vec3<T> &b) const {
    return (a.x * a.x + a.y * a.y + a.z * a.z) <
           (b.x * b.x + b.y * b.y + b.z * b.z);
  }
};

template <typename T> struct Vec3Greater {
  bool operator()(const Vec3<T> &a, const Vec3<T> &b) const {
    return (a.x * a.x + a.y * a.y + a.z * a.z) >
           (b.x * b.x + b.y * b.y + b.z * b.z);
  }
};

void printVec3(const Vec3<float> &v) {
  printf("(%.0f,%.0f,%.0f) ", v.x, v.y, v.z);
}

// Classe com destrutor
class Traced {
  char *_label;
  int _id;
  static int _liveCount;

public:
  Traced(int id, const char *label) : _id{id} {
    _label = new char[strlen(label) + 1];
    strcpy(_label, label);
    _liveCount++;
  }

  Traced(const Traced &other) : _id{other._id} {
    _label = new char[strlen(other._label) + 1];
    strcpy(_label, other._label);
    _liveCount++;
  }

  ~Traced() {
    _liveCount--;
    delete[] _label;
  }

  auto id() const { return _id; }
  auto label() const { return _label; }
  static auto liveCount() { return _liveCount; }
};

int Traced::_liveCount = 0;

struct TracedLess {
  bool operator()(const Traced &a, const Traced &b) const {
    return a.id() < b.id();
  }
};

struct TracedGreater {
  bool operator()(const Traced &a, const Traced &b) const {
    return a.id() > b.id();
  }
};

void printTraced(const Traced &t) { printf("{%d,\"%s\"} ", t.id(), t.label()); }

//  Funcoes de teste
void testIntLess() {
  runFullTest<int, std::less<int>>(
      "int, std::less<int>", {5, 3, 7, 9, 8, 0, 4, 2, 1, 6}, 7, 5, printInt);
}

void testIntGreater() {
  runFullTest<int, std::greater<int>>(
      "int, std::greater<int>", {5, 3, 7, 9, 8, 0, 4, 2, 1, 6}, 8, 8, printInt);
}

void testVecLess() {
  using V = Vec3<float>;
  runFullTest<V, Vec3Less<float>>(
      "Vec3<float>, Vec3Less<float>",
      {{1, 0, 0}, {1, 1, 1}, {2, 0, 0}, {1, 1, 0}, {3, 0, 0}}, V{2, 0, 0},
      V{2, 0, 0}, printVec3);
}

void testVecGreater() {
  using V = Vec3<float>;
  runFullTest<V, Vec3Greater<float>>("Vec3<float>, Vec3Greater<float>",
                                     {{1, 0, 0}, {1, 1, 1}, {2, 0, 0}},
                                     V{1, 1, 1}, V{1, 0, 0}, printVec3);
}

void testTracedLess() {
  printf("--- Traced, TracedLess ---\n");
  printf("live objects: %d\n", Traced::liveCount());

  {
    avl::Tree<Traced, TracedLess> t;
    t.insert(Traced{3, "gamma"});
    t.insert(Traced{1, "alpha"});
    t.insert(Traced{2, "beta"});
    printf("insert(3):   size=%d  height=%d  live=%d\n", t.size(), t.height(),
           Traced::liveCount());

    printf("iterate:     ");
    t.iterate(printTraced);
    puts("");

    printf("find(id=2):  %s\n",
           t.find(Traced{2, "?"}) != t.end() ? "found" : "not found");

    t.erase(Traced{2, "?"});
    printf("erase(id=2): size=%d  live=%d\n", t.size(), Traced::liveCount());

    printf("iterate:     ");
    t.iterate(printTraced);
    puts("");
    printf("~Tree:       destroying scope...\n");
  }

  printf("live objects: %d (expected 0)\n\n", Traced::liveCount());
}

void testTracedGreater() {
  printf("--- Traced, TracedGreater ---\n");
  printf("live objects: %d\n", Traced::liveCount());

  {
    avl::Tree<Traced, TracedGreater> t;
    t.insert(Traced{10, "dez"});
    t.insert(Traced{20, "vinte"});
    printf("insert(2):   size=%d  live=%d\n", t.size(), Traced::liveCount());

    printf("iterate:     ");
    t.iterate(printTraced);
    puts("");

    t.clear();
    printf("clear:       size=%d  empty=%s  live=%d\n", t.size(),
           t.empty() ? "true" : "false", Traced::liveCount());
  }

  printf("live objects: %d (expected 0)\n\n", Traced::liveCount());
}

//  main
int main(int argc, char **argv) {
  std::string type = "all";
  std::string functor = "all";
  if (argc > 1)
    type = argv[1];
  if (argc > 2)
    functor = argv[2];

  bool runInt = (type == "all" || type == "int");
  bool runVec = (type == "all" || type == "vec");
  bool runTraced = (type == "all" || type == "traced");
  bool runLess = (functor == "all" || functor == "less");
  bool runGreater = (functor == "all" || functor == "greater");

  printf("avl::Tree test suite  [type=%s, functor=%s]\n\n", type.c_str(),
         functor.c_str());

  if (runInt && runLess)
    testIntLess();
  if (runInt && runGreater)
    testIntGreater();
  if (runVec && runLess)
    testVecLess();
  if (runVec && runGreater)
    testVecGreater();
  if (runTraced && runLess)
    testTracedLess();
  if (runTraced && runGreater)
    testTracedGreater();

  puts("done.");
  return 0;
}
