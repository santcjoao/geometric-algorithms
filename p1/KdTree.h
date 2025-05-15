#ifndef __KdTree_h
#define __KdTree_h

#include "graphics/Bounds3.h"
#include <concepts>

namespace tcii::p1
{ // begin namespace tcii::p1

using namespace cg;

template <typename T, typename U>
concept SameAs = std::same_as<T, U>;

template <typename R>
concept IsReal = std::is_floating_point_v<R>;

template <typename C, typename E>
concept IsIndexable = requires (C x, size_t i)
{
  { x[i] } -> std::convertible_to<E>;
};

template <template <size_t, typename> typename P, typename R, size_t D>
concept IsPoint = IsReal<R> &&
  SameAs<R, typename P<D, R>::value_type> &&
  IsIndexable<P<D, R>, R> &&
requires (P<D, R> p, R r)
{
  { p + p } -> SameAs<P<D, R>>;
  { p - p } -> SameAs<P<D, R>>;
  { p * r } -> SameAs<P<D, R>>;
  { r * p } -> SameAs<P<D, R>>;
};

template <typename PA,
  template <size_t, typename> typename P,
  typename R,
  size_t D>
concept IsPointArray = IsPoint<P, R, D> &&
  IsIndexable<PA, P<D, R>> &&
  std::move_constructible<PA> &&
requires (PA a, size_t i)
{
  { a.size() } -> SameAs<size_t>;
};

template <size_t D, typename real, IsPointArray<Vec, real, D> PA>
auto
computeBounds(const PA& points)
{
  Bounds<D, real> b;

  for (size_t n = points.size(), i = 0; i < n;)
    b.inflate(points[i++]);
  return b;
}

template <size_t D, typename real, IsPointArray<Vec, real, D> PA>
class PointHolder
{
public:
  PointHolder(PA&& points):
    _points{std::move(points)}
  {
    // do nothing
  }

  auto& points() const
  {
    return _points;
  }

protected:
  PA _points;

}; // PointHolder

template <size_t D, typename real, typename PA>
class KdTree: public PointHolder<D, real, PA>
{
public:
  using Base = PointHolder<D, real, PA>;

  const struct Params
  {
    size_t maxPointsPerNode;
    size_t minPointsPerNode;
    size_t maxDepth;

  } params;

  static constexpr Params dflParams()
  {
    return {20, 5, 8};
  }

  KdTree(PA&& points, const Params& params = dflParams());

private:
  struct Node;

}; // KdTree

template <size_t D, typename real, typename PA>
KdTree<D, real, PA>::KdTree(PA&& points, const Params& params):
  Base{std::move(points)},
  params{params}
{
  // TODO
}

template <typename real, typename PA> using KdTree3 = KdTree<3, real, PA>;

} // end namespace tcii::p1

#endif // __KdTree_h
