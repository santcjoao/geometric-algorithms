#include "./src/Types.h"
#include "./src/Bench.h"
#include "./src/Parse.h"

#include <cstdlib>

auto funcAll = [](const auto&, size_t) { 
  return true; 
};

auto funcEarly = [](const auto&, size_t) { 
  return false; 
};

auto funcCustom = [](const auto&, size_t pid) { 
  static size_t processedNeighbors = 0; 
  return (pid % 2 == 0) && (++processedNeighbors < 10); 
};

int main(int argc, char** argv)
{
  Config cfg = parseArguments(argc, argv);

  auto dispatch = [&](auto func) 
  {
    if (cfg.iter) {
      bool printHdr = cfg.printHeader;
      for (size_t n = cfg.iter->start; n <= cfg.iter->end; n += cfg.iter->stride) {
        Benchmarking::TestReport r;
        if (cfg.gen.container == StructType::Vector)
          r = Benchmarking::executeCore<std::vector<Point>, RangeTreeVector>(n, cfg, func);
        else
          r = Benchmarking::executeCore<PointArray, RangeTreeArray>(n, cfg, func);
        
        Benchmarking::printCsv(r, printHdr);
        printHdr = false;
      }
    } else {
      Benchmarking::TestReport r;
      if (cfg.gen.container == StructType::Vector)
        r = Benchmarking::executeCore<std::vector<Point>, RangeTreeVector>(cfg.gen.count, cfg, func);
      else
        r = Benchmarking::executeCore<PointArray, RangeTreeArray>(cfg.gen.count, cfg, func);
      
      Benchmarking::printConsole(r);
    }
  };

  if (cfg.proc.func == FuncType::Early)    dispatch(funcEarly);
  else if (cfg.proc.func == FuncType::Custom) dispatch(funcCustom);
  else                                     dispatch(funcAll);

  return EXIT_SUCCESS;
}