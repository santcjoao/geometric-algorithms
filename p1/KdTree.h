#ifndef __KdTree_h
#define __KdTree_h

#include "Utils.h"
#include <utility>

namespace tcii::p1
{ // begin namespace tcii::p1

using namespace cg;

template <size_t D, IsReal R, IsPointArray<Vec, R, D> A>
class PointHolder
{
public:
  using Point = Vec<D, R>;

  PointHolder(A&& points):
    _points{std::move(points)}
  {
    // do nothing
  }

  auto& points() const
  {
    return _points;
  }

private:
  A _points;

}; // PointHolder

template <size_t D, typename R, typename A>
class KdTree: public PointHolder<D, R, A>
{
public:
  using Base = PointHolder<D, R, A>;
  using Bounds = cg::Bounds<D, R>;

  const struct Params
  {
    unsigned maxPointsPerNode;
    unsigned minPointsPerNode;
    unsigned maxDepth;

  } params;

  static constexpr Params dflParams()
  {
    return {20, 5, 8};
  }

  KdTree(A&& points, const Params& params = dflParams());

  KdTree(const KdTree&) = delete;
  KdTree& operator =(const KdTree&) = delete;

  KdTree(KdTree&& tree) noexcept:
    _root{std::exchange(tree._root, nullptr)},
    _nodeCount{tree._nodeCount},
    _leafCount{tree._leafCount},
    params{tree.params}
  {
    // do nothing
  }

  ~KdTree()
  {
    delete _root;
  }

  auto& bounds() const
  {
    return _root->bounds;
  }

  auto nodeCount() const
  {
    return _nodeCount;
  }

  auto leafCount() const
  {
    return _leafCount;
  }

private:
  struct Node
  {
    Bounds bounds;

    Node(const Bounds& bounds, unsigned depth):
      bounds{bounds}
    {
      // TODO
    }

  }; // Node

  Node* _root{};
  unsigned _nodeCount{};
  unsigned _leafCount{};

}; // KdTree

template <size_t D, typename R, typename A>
KdTree<D, R, A>::KdTree(A&& points, const Params& params):
  Base{std::move(points)},
  params{params}
{
  _root = new Node{computeBounds<D, R>(this->points()), 0};
  _nodeCount = _leafCount = 1;
  // TODO
}

template <typename R, typename A> using KdTree3 = KdTree<3, R, A>;

} // end namespace tcii::p1

#endif // __KdTree_h
