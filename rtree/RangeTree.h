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

  void build(index_t first, index_t last, const IndexArray& indices, const A& points)
  {}

  size_t query(const A& points, const Bounds& bounds, PointFunc f) const
  {
    // insert your code here
    return 0;
  }

private:
  

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

    // initializing indices array
    _indices{points};

    // sort the index array (canonic set of node) by their d-th coordinate
    std::sort(_indices.begin(), _indices.end(), [&points](index_t i, size_t j){
       return points[i][D] < points[j][D] });

    index_t first = 0;
    index_t last = (index_t)_indices.size();
    build(_root, first, last, _indices, points);
  }

 void build(index_t first, index_t last, const IndexArray& indices, const A& points)
 {
    assert(!_root);

    // initializing index array
    _indices{last - first};

    for (size_t i = 0, j = first; i < last - first && j < last; i++, j++)
      _indices[i] = indices[j];

    // sorting by d-th coordinate and building
    std::sort(_indices.begin(), _indices.end(), [&points](index_t i, size_t j){
      return points[i][D] < points[j][D]});
    
    index_t first = 0;
    index_t last = (index_t)_indices.size();
    build(_root, first, last, _indices, points);
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

  void build (Node*& node, index_t first, index_t last, const IndexArray& indices, const A& points);
 
  Node* _root{};
  IndexArray _indices;

}; // BBST

template<size_t D, typename P, typename A>
void BBST<D, P, A> :: build(Node*& node, index_t first, index_t last, const IndexArray& indices, const A& points)
{
  if (first == last)
    return;
  else if (last != first + 1)
  {
    // create a new node
    index_t median = last / 2;
    node = new Node(points[indices[median]][D], first, last);

    // searching new first and new last
    index_t nf;
    index_t nl;
    for (nf = median + 1; points[_indices[nf]][D] != points[_indices[median]][D] && nf != last; nf++);
    for (nl = median - 1; points[_indices[nl]][D] != points[_indices[median]][D] && nl != first; nl--);

    // recursive step
    build(node->left, first, nl, indices, points);
    build(node->right, nf, last, indices, points);
  }
  else
    node =  new Node(points[indices[first]][D], first, last);

  // building associated trees
  if (node != _root)
    node->associatedTree->build(node->_first, node->_last, indices, points);
  else
    node->associatedTree->build(points);
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
