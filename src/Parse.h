#ifndef __PARSE_H
#define __PARSE_H

#include <string>
#include <string_view>
#include <iostream>
#include <cstdlib>
#include <optional>
#include <cstdint>

enum class AlgoType { Naive, Range, Both };
enum class FuncType { All, Early, Custom };
enum class StructType { Array, Vector };

struct ProcessingConfig 
{
  AlgoType algo = AlgoType::Both;
  FuncType func = FuncType::All;
};

struct GenerationConfig 
{
  StructType container = StructType::Array;
  size_t count         = 50000;
  size_t dim           = 3;
  float box            = 200.0f;
  uint64_t seed        = 123456789ULL;
};

struct IterationConfig 
{
  size_t start  = 0;
  size_t end    = 0;
  size_t stride = 0;
};

struct Config 
{
  ProcessingConfig proc;
  GenerationConfig gen;
  std::optional<IterationConfig> iter;
  bool printHeader = true;
};


inline size_t parseK(std::string_view val)
{
  if (val.empty()) return 0;
  size_t multiplier = 1;
  if (val.back() == 'k' || val.back() == 'K') {
    multiplier = 1000;
    val.remove_suffix(1);
  }
  return std::stoull(std::string(val)) * multiplier;
}

inline float parseFloat(std::string_view val)
{
  if (val.empty()) return 0.0f;
  return std::stof(std::string(val));
}


inline void extractProcessing(ProcessingConfig& proc, std::string_view key, std::string_view val)
{
  if (key == "algo") {
    if (val == "naive") proc.algo = AlgoType::Naive;
    else if (val == "range") proc.algo = AlgoType::Range;
    else proc.algo = AlgoType::Both;
  } 
  else if (key == "func") {
    if (val == "early") proc.func = FuncType::Early;
    else if (val == "custom") proc.func = FuncType::Custom;
    else proc.func = FuncType::All;
  }
}


inline void extractGeneration(GenerationConfig& gen, std::string_view key, std::string_view val)
{
  if (key == "struct") {
    if (val == "vector") gen.container = StructType::Vector;
    else gen.container = StructType::Array;
  }
  else if (key == "count") gen.count = parseK(val);
  else if (key == "dim")   gen.dim = parseK(val);
  else if (key == "box")   gen.box = parseFloat(val);
  else if (key == "seed")  gen.seed = parseK(val);
}

inline void validateGeneration(GenerationConfig& gen)
{
  if (gen.dim > 3) {
    std::cout << "[AVISO] Dimensão " << gen.dim << " excede o limite. Clamping para 3 efetuado.\n";
    gen.dim = 3;
  }
}


inline void extractIteration(IterationConfig& iter, bool& hasIter, std::string_view key, std::string_view val)
{
  if (key == "start")  { iter.start = parseK(val); hasIter = true; }
  else if (key == "end")    { iter.end = parseK(val); hasIter = true; }
  else if (key == "stride") { iter.stride = parseK(val); hasIter = true; }
}

inline std::optional<IterationConfig> validateIteration(const IterationConfig& iter, bool hasIter, const ProcessingConfig& proc)
{
  if (!hasIter) return std::nullopt;

  if (proc.algo != AlgoType::Both) {
    std::cout << "[AVISO] Profiling ignorado. A iteracao exige 'algo=both'.\n";
    return std::nullopt;
  }

  if (iter.start < iter.end && iter.stride > 0) {
    return iter;
  }

  return std::nullopt;
}


inline Config parseArguments(int argc, char** argv)
{
  Config cfg;
  IterationConfig tempIter;
  bool hasIter = false;

  for (int i = 1; i < argc; ++i)
  {
    std::string_view arg(argv[i]);
    size_t pos = arg.find('=');
    if (pos == std::string_view::npos) continue;

    std::string_view key = arg.substr(0, pos);
    std::string_view val = arg.substr(pos + 1);

    extractProcessing(cfg.proc, key, val);
    extractGeneration(cfg.gen, key, val);
    extractIteration(tempIter, hasIter, key, val);
    if (key == "header") {
      if (val == "false" || val == "0") cfg.printHeader = false;
    }
  }

  validateGeneration(cfg.gen);
  cfg.iter = validateIteration(tempIter, hasIter, cfg.proc);

  return cfg;
}

#endif