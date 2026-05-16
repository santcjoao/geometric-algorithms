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
    // initializing indices array
    _indices{points};

    // sort the index array (canonic set of node) by their d-th coordinate
    std::sort(_indices.begin(), _indices.end(), [&points](int i, int j){
       return points[i][D] < points[j][D] });

    // get the median value to be the root
    size_t median = _indices.size()/2;
    _root = new Node(points[_indices[median]][D], 0, _indices.size());

    // build the left and right subtrees
    build(_root->left, 0, median, _indices, points);
    build(_root->right, median + 1, _indices.size(), _indices, points);
    
    // build the associated tree related to this node
    _root->associatedTree->build(points);
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
    real _splitValue; // Pivô. Valor da d-ésima coordenada do ponto.

    // Intervalo [first, last) de pontos associados a este conjunto nodal
    index_t _first;
    index_t _last;

    AssociatedTree* associatedTree{};
    Node* left{};
    Node* right{};

    Node() = default;
    
    Node(real value):
      _splitValue{value}
    {
      // Do nothing
    }

    Node(real value, index_t first, index_t last):
      _splitValue{value}, 
      _first{first}, 
      _last{last}
    {
      // Do nothing
    }
  }; // Node

  void build (Node*& child, size_t first, size_t last, const IndexArray& indices, const A& points);

  Node* _root{};
  IndexArray _indices;

}; // BBST

template<size_t D, typename P, typename A>
void BBST<D, P, A> :: build(Node*& child, size_t first, size_t last, const IndexArray& indices, const A& points)
{
  if (first == last)
    return;
  else if (last != first + 1)
  {
    // create a new node
    size_t median = last / 2;
    child = new Node(points[indices[median]][D], first, last);

    // recursive step
    build(child->left, first, median, indices, points);
    build(child->right, median + 1, last, indices, points);
  }
  else
  {
    child =  new Node(points[indices[first]][D], first, last);
  }
  child->associatedTree->build(points);
}

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
