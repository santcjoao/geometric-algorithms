#ifndef __Utils_h
#define __Utils_h

#include "graphics/Vec3.h"
#include <vector>

namespace tcii::p1
{ // begin namespace tcii::p1

using namespace cg;
using Points3f = std::vector<Vec3f>;

template <typename real>
inline auto
srand()
{
  return (real)((double)rand() / RAND_MAX);
}

template <typename real>
inline auto
srand(real min, real max)
{
  return min + srand<real>() * (max - min);
}

template <size_t D, typename real>
inline auto
prand(real min, real max)
{
  Vec<D, real> p;

  for (size_t i = 0; i < D;)
    p[i++] = srand(min, max);
  return p;
}

inline auto
prand(size_t n, float min = 0, float max = 1)
{
  assert(n > 0);

  Points3f p(n);

  for (size_t i = 0; i < n;)
    p[i++] = p1::prand<3>(min, max);
  return p;
}

} // end namespace tcii::p1

#endif // __Utils_h