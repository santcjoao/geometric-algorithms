#ifndef __RangeTree_h
#define __RangeTree_h

#include "util/Array.h"
#include "PointTraits.h"
#include <functional>
#include <numeric>

namespace tcii::cg
{ // begin namespace tcii::cg

namespace rtree
{ // begin namespace rtree

using index_t = unsigned;
using IndexArray = Array<index_t>;

template <size_t D, typename P>
inline auto _x(const P& p)
{
  if constexpr (std::is_arithmetic_v<P>)
  {
    static_assert(D == 1);
    return p;
  }
  else
    return p[D - 1];
}

template <typename A>
using PointFunc = std::function<bool(const A&, size_t)>;

template <size_t D, typename P, typename A> class BBST;

template <typename P, typename A>
class BBST<1, P, A>
{
public:
  using Bounds = typename PointTraits<P>::Bounds;
  using PointFunc = rtree::PointFunc<A>;

  void build(const A& points)
  {
    // insert your code here
  }
 
  size_t query(const A& points, const Bounds& bounds, PointFunc f) const
  {
    // insert your code here
    return 0;
  }

}; // BBST

template <size_t D, typename P, typename A>
class BBST
{
public:
  using Bounds = typename PointTraits<P>::Bounds;
  using PointFunc = rtree::PointFunc<A>;

  ~BBST()
  {
    delete _root;
  }

  void build(const A& points)
  {
    assert(!_root);
    // insert your code here
  }

  size_t query(const A& points, const Bounds& bounds, PointFunc f) const
  {
    // insert your code here
    return 0;
  }

private:
  using real = typename P::value_type;
  using AssociatedTree = BBST<D - 1, P, A>;

  struct Node
  {
    // insert your code here

  }; // Node

  Node* _root{};
  IndexArray _indices;

}; // BBST

} // end namespace rtree

template <typename P, typename A>
class RangeTree
{
public:
  constexpr static auto D = point_dim_v<P>;

  using Bounds = typename PointTraits<P>::Bounds;
  using PointFunc = rtree::PointFunc<A>;

  RangeTree(const A& points):
    _points{points}
  {
    // do nothing
  }

  auto& points() const
  {
    return _points;
  }

  void build()
  {
    _mainTree.build(_points);
  }

  auto query(const Bounds& bounds, PointFunc f) const
  {
    return _mainTree.query(_points, bounds, f);
  }

private:
  const A& _points;
  rtree::BBST<D, P, A> _mainTree;

}; // RangeTree

} // end namespace tcii::cg

#endif // __RangeTree_h
