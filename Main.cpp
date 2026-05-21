#include "include/graphics/Bounds3.h"
#include "src/RangeTree.h"

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <random>

#ifdef _WIN32
  #include <windows.h>
  #include <psapi.h>
#else
  #include <sys/resource.h>
#endif

using namespace tcii;

using Point = cg::Vec3f;
using PointArray = cg::Array<Point>;
using Bounds = cg::Bounds3f;

template <>
struct cg::PointTraits<Point>
{
  static constexpr size_t dim = 3;
  using Bounds = ::Bounds;
};

using RangeTreeArray = cg::RangeTree<Point, PointArray>;
using RangeTreeVector = cg::RangeTree<Point, std::vector<Point>>;

namespace
{

struct BenchResult
{
  double naiveMs{};
  double buildMs{};
  double queryMs{};
  size_t naiveCount{};
  size_t rtCount{};
  size_t peakRssKb{};
};

size_t getPeakRSS()
{
  #ifdef _WIN32
    PROCESS_MEMORY_COUNTERS info;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info)))
      return static_cast<size_t>(info.PeakWorkingSetSize / 1024); // Converte bytes para KB
    return 0;
  #else
    rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) == 0)
      return static_cast<size_t>(usage.ru_maxrss); // No Linux já retorna em KB
    return 0;
  #endif
}

Bounds makeDomain(int mode)
{
  Bounds domain;
  if (mode == 2)
  {
    domain.inflate({0.0f, 0.0f, 0.0f});
    domain.inflate({1000.0f, 1000.0f, 0.0f});
  }
  else
  {
    domain.inflate({-1000.0f, -1000.0f, -1000.0f});
    domain.inflate({1000.0f, 1000.0f, 1000.0f});
  }
  return domain;
}

Bounds makeQueryBox(int mode, float boxSize)
{
  Bounds queryBox;
  if (mode == 2)
  {
    queryBox.inflate({500.0f - boxSize / 2.0f, 500.0f - boxSize / 2.0f, 0.0f});
    queryBox.inflate({500.0f + boxSize / 2.0f, 500.0f + boxSize / 2.0f, 0.0f});
  }
  else
  {
    queryBox.inflate({-boxSize / 2.0f, -boxSize / 2.0f, -boxSize / 2.0f});
    queryBox.inflate({boxSize / 2.0f, boxSize / 2.0f, boxSize / 2.0f});
  }
  return queryBox;
}

PointArray makePoints(size_t n, const Bounds& domain, std::uint64_t seed)
{
  using Real = Point::value_type;
  PointArray points(n);
  std::mt19937_64 gen(seed);
  std::uniform_real_distribution<Real> distX{domain[0].x, domain[1].x};
  std::uniform_real_distribution<Real> distY{domain[0].y, domain[1].y};
  std::uniform_real_distribution<Real> distZ{domain[0].z, domain[1].z};

  for (size_t i = 0; i < n; ++i)
  {
    points[i].x = distX(gen);
    points[i].y = distY(gen);
    points[i].z = distZ(gen);
  }
  return points;
}

size_t naiveQuery(const PointArray& points, const Bounds& bounds)
{
  size_t count{};
  for (size_t i = 0; i < points.size(); ++i)
    if (bounds.contains(points[i]))
      ++count;
  return count;
}

// Template para permitir testes com cg::Array e std::vector
template <typename Container, typename TreeType>
BenchResult runBenchmark(int mode, size_t n, float boxSize, std::uint64_t seed)
{
  BenchResult result;
  const auto domain = makeDomain(mode);
  const auto queryBox = makeQueryBox(mode, boxSize);

  // Gera dados base e converte para o container desejado
  const auto rawPoints = makePoints(n, domain, seed);
  Container points(n);
  for (size_t i = 0; i < n; ++i) points[i] = rawPoints[i];

  // Naive
  const auto t0 = std::chrono::steady_clock::now();
  result.naiveCount = naiveQuery(rawPoints, queryBox);
  const auto t1 = std::chrono::steady_clock::now();
  result.naiveMs = std::chrono::duration<double, std::milli>(t1 - t0).count();

  // Range Tree Build
  TreeType tree{points};
  const auto t2 = std::chrono::steady_clock::now();
  tree.build();
  const auto t3 = std::chrono::steady_clock::now();
  result.buildMs = std::chrono::duration<double, std::milli>(t3 - t2).count();

  // Range Tree Query
  const auto countAll = [](const Container&, size_t) { return true; };
  const auto t4 = std::chrono::steady_clock::now();
  result.rtCount = tree.query(queryBox, countAll);
  const auto t5 = std::chrono::steady_clock::now();
  result.queryMs = std::chrono::duration<double, std::milli>(t5 - t4).count();

  result.peakRssKb = getPeakRSS();
  return result;
}

} // namespace

int main(int argc, char** argv)
{
  // Argumentos: ./benchmark [Mode: 2|3] [N] [BoxSize] [Container: 0=Array, 1=Vector] [CSV: 0|1] [Seed]
  const int mode = (argc > 1) ? std::stoi(argv[1]) : 3;
  const size_t n = (argc > 2) ? std::stoull(argv[2]) : 50000;
  const float boxSize = (argc > 3) ? std::stof(argv[3]) : 200.0f;
  const int useVector = (argc > 4) ? std::stoi(argv[4]) : 0;
  const int exportCSV = (argc > 5) ? std::stoi(argv[5]) : 0;
  const std::uint64_t seed = (argc > 6) ? std::stoull(argv[6]) : 123456789ULL;

  BenchResult result;
  if (useVector)
    result = runBenchmark<std::vector<Point>, RangeTreeVector>(mode, n, boxSize, seed);
  else
    result = runBenchmark<PointArray, RangeTreeArray>(mode, n, boxSize, seed);

  const bool correct = (result.naiveCount == result.rtCount);

  if (exportCSV)
  {
    // Formato CSV: Mode,Container,N,BoxSize,Seed,NaiveCount,RTCount,Correct,NaiveMs,BuildMs,QueryMs,PeakRSS_KB
    std::printf("%d,%d,%zu,%g,%llu,%zu,%zu,%d,%.4f,%.4f,%.4f,%zu\n",
                mode, useVector, n, boxSize, static_cast<unsigned long long>(seed),
                result.naiveCount, result.rtCount, correct ? 1 : 0,
                result.naiveMs, result.buildMs, result.queryMs, result.peakRssKb);
  }
  else
  {
    std::puts("==================================================");
    std::printf("RANGE TREE BENCHMARK (Dim: R%d, N: %zu, Type: %s)\n", mode, n, useVector ? "std::vector" : "cg::Array");
    std::puts("==================================================");
    std::printf("[-] Seed                 : %llu\n", static_cast<unsigned long long>(seed));
    std::printf("[-] Naive Search         : %.4f ms (%zu points)\n", result.naiveMs, result.naiveCount);
    std::printf("[-] Range Tree Build     : %.4f ms\n", result.buildMs);
    std::printf("[-] Range Tree Query     : %.4f ms (%zu points)\n", result.queryMs, result.rtCount);
    std::printf("[-] Peak Memory (RSS)    : %zu KB\n", result.peakRssKb);
    std::printf("[-] Integrity Check      : %s\n", correct ? "SUCCESS" : "FAILURE");
    std::puts("==================================================");
  }

  return correct ? EXIT_SUCCESS : EXIT_FAILURE;
}