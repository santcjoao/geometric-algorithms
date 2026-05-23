#ifndef TCII_CG_TYPES_H
#define TCII_CG_TYPES_H

#include "../include/graphics/Bounds3.h"
#include "../include/graphics/Vec3.h"
#include "../include/util/Array.h"
#include "RangeTree.h"

#include <vector>

using Point = tcii::cg::Vec3f;
using Bounds = tcii::cg::Bounds3f;
using PointArray = tcii::cg::Array<Point>;

namespace tcii::cg 
{
  template <>
  struct PointTraits<Point> {
    static constexpr size_t dim = 3;
    using Bounds = ::Bounds;
  };
}

using RangeTreeArray  = tcii::cg::RangeTree<Point, PointArray>;
using RangeTreeVector = tcii::cg::RangeTree<Point, std::vector<Point>>;

#endif // TCII_CG_TYPES_H