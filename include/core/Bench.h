#ifndef TCII_CG_BENCH_H
#define TCII_CG_BENCH_H

#include "core/Generator.h"
#include "core/Parse.h"

#include <chrono>
#include <cstdint>
#include <cstdio>

#ifdef _WIN32
#include <psapi.h>
#include <windows.h>
#else
#include <sys/resource.h>
#endif

namespace Benchmarking {

struct PhaseMetrics {
  double timeMs = 0.0;
  size_t memoryPeakRssKb = 0;
};

struct AlgorithmResult {
  size_t pointsFound = 0;
  PhaseMetrics build;
  PhaseMetrics query;
};

struct TestReport {
  size_t pointVolume;
  size_t dimension;
  float boxSize;
  StructType container;

  AlgorithmResult naive;
  AlgorithmResult rangeTree;
  bool correspondenceMatch = false;
};

inline size_t getPeakMemory() {
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS info;
  if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info)))
    return static_cast<size_t>(info.PeakWorkingSetSize / 1024);
  return 0;
#else
  rusage usage{};
  if (getrusage(RUSAGE_SELF, &usage) == 0)
    return static_cast<size_t>(usage.ru_maxrss);
  return 0;
#endif
}

inline void printConsole(const TestReport &r) {
  std::puts("--------------------------------------------------");
  std::printf(" REPORT [%s | %zuD]\n",
              (r.container == StructType::Vector ? "std::vector" : "cg::Array"),
              r.dimension);
  std::puts("--------------------------------------------------");
  std::printf(" Dataset Volume        : %zu points\n", r.pointVolume);
  std::printf(" Query Window (AABB)   : %g\n", r.boxSize);

  if (r.naive.query.timeMs > 0.0)
    std::printf(" Naive Query           : %.4f ms (Found: %zu)\n",
                r.naive.query.timeMs, r.naive.pointsFound);

  if (r.rangeTree.query.timeMs > 0.0) {
    std::printf(" Range Tree Build      : %.4f ms [Peak RSS: %zu KB]\n",
                r.rangeTree.build.timeMs, r.rangeTree.build.memoryPeakRssKb);
    std::printf(
        " Range Tree Query      : %.4f ms (Found: %zu) [Peak RSS: %zu KB]\n",
        r.rangeTree.query.timeMs, r.rangeTree.pointsFound,
        r.rangeTree.query.memoryPeakRssKb);
  }

  std::printf(" Naive/RT Correspondence : %s\n",
              r.correspondenceMatch ? "MATCH" : "MISMATCH");
  std::puts("--------------------------------------------------");
}

inline void printCsv(const TestReport &r, bool printHeader = false) {
  if (printHeader)
    std::puts(
        "N,Dim,Box,Struct,Match,NaiveMs,RtBuildMs,RtQueryMs,RtQueryRssKb");

  std::printf("%zu,%zu,%g,%d,%d,%.4f,%.4f,%.4f,%zu\n", r.pointVolume,
              r.dimension, r.boxSize, static_cast<int>(r.container),
              r.correspondenceMatch ? 1 : 0, r.naive.query.timeMs,
              r.rangeTree.build.timeMs, r.rangeTree.query.timeMs,
              r.rangeTree.query.memoryPeakRssKb);
}

template <typename Container, typename Predicate>
size_t queryNaive(const Container &points, const Bounds &box, Predicate f) {
  size_t count = 0;
  for (size_t i = 0; i < points.size(); ++i)
    if (box.contains(points[i])) {
      count++;
      if (!f(points[i], i))
        break;
    }

  return count;
}

template <typename Container, typename TreeType, typename Predicate>
TestReport executeCore(size_t volume, const Config &cfg, Predicate p) {
  TestReport report;
  report.pointVolume = volume;
  report.dimension = cfg.gen.dim;
  report.boxSize = cfg.gen.box;
  report.container = cfg.gen.container;

  const auto box = calculateBox(cfg.gen.dim, cfg.gen.box);
  const auto rawPoints = generatePoints(volume, cfg.gen.dim, cfg.gen.seed);

  Container points(volume);
  for (size_t i = 0; i < volume; ++i)
    points[i] = rawPoints[i];

  if (cfg.proc.algo == AlgoType::Naive || cfg.proc.algo == AlgoType::Both) {
    auto t0 = std::chrono::steady_clock::now();
    report.naive.pointsFound = queryNaive(points, box, p);
    auto t1 = std::chrono::steady_clock::now();
    report.naive.query.timeMs =
        std::chrono::duration<double, std::milli>(t1 - t0).count();
    report.naive.query.memoryPeakRssKb = getPeakMemory();
  }

  if (cfg.proc.algo == AlgoType::Range || cfg.proc.algo == AlgoType::Both) {
    TreeType tree{points};

    auto t0 = std::chrono::steady_clock::now();
    tree.build();
    auto t1 = std::chrono::steady_clock::now();
    report.rangeTree.build.timeMs =
        std::chrono::duration<double, std::milli>(t1 - t0).count();
    report.rangeTree.build.memoryPeakRssKb = getPeakMemory();

    auto t2 = std::chrono::steady_clock::now();
    tcii::cg::rtree::PointFunc<Container> func = p;
    report.rangeTree.pointsFound = tree.query(box, func);
    auto t3 = std::chrono::steady_clock::now();
    report.rangeTree.query.timeMs =
        std::chrono::duration<double, std::milli>(t3 - t2).count();
    report.rangeTree.query.memoryPeakRssKb = getPeakMemory();
  }

  report.correspondenceMatch =
      (cfg.proc.algo == AlgoType::Both)
          ? (report.naive.pointsFound == report.rangeTree.pointsFound)
          : true;
  return report;
}

} // namespace Benchmarking

#endif