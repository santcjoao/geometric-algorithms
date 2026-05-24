#ifndef TCII_CG_GENERATOR_H
#define TCII_CG_GENERATOR_H

#include "../include/graphics/Bounds3.h"
#include "../include/graphics/Vec3.h"
#include "../include/util/Array.h"
#include <random>

namespace Benchmarking 
{

using Point = tcii::cg::Vec3f;
using Bounds = tcii::cg::Bounds3f;
using PointArray = tcii::cg::Array<Point>;

inline PointArray generatePoints(size_t n, size_t dim, uint64_t seed)
{
  Bounds domain;

  if (dim == 2)
  {
    domain.inflate({0.0f, 0.0f, 0.0f});
    domain.inflate({1000.0f, 1000.0f, 0.0f});
  }
  else
  {
    domain.inflate({-1000.0f, -1000.0f, -1000.0f});
    domain.inflate({1000.0f, 1000.0f, 1000.0f});
  }

  PointArray points(n);
  std::mt19937_64 gen(seed);
  std::uniform_real_distribution<float> distX{domain[0].x, domain[1].x};
  std::uniform_real_distribution<float> distY{domain[0].y, domain[1].y};
  std::uniform_real_distribution<float> distZ{domain[0].z, domain[1].z};

  for (size_t i = 0; i < n; ++i)
  {
    points[i].x = distX(gen);
    points[i].y = distY(gen);
    points[i].z = (dim == 2) ? 0.0f : distZ(gen);
  }
  return points;
}

inline Bounds calculateBox(size_t dim, float size)
{
  Bounds box;
  if (dim == 2)
  {
    box.inflate({500.0f - size / 2.0f, 500.0f - size / 2.0f, 0.0f});
    box.inflate({500.0f + size / 2.0f, 500.0f + size / 2.0f, 0.0f});
  }
  else
  {
    box.inflate({-size / 2.0f, -size / 2.0f, -size / 2.0f});
    box.inflate({size / 2.0f, size / 2.0f, size / 2.0f});
  }
  return box;
}

}

#endif