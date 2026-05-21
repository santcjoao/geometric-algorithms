#ifndef __RangeTree_h
#define __RangeTree_h

#include "util/Array.h"
#include "PointTraits.h"
#include <functional>
#include <numeric>
#include <iostream>

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
    assert(_indices.size() == 0);
    _indices = IndexArray(points.size());
    for (index_t i = 0; i < points.size(); ++i)
    {
      _indices[i] = i;
    }
    sortIndices(points);
  }

  void build(index_t first, index_t last, const IndexArray& indices, const A& points)
  {
    assert(_indices.size() == 0);
    _indices = IndexArray(last - first);

    for(auto i = 0u, j = first; i < last - first && j < last; i++, j++)
      _indices[i] = indices[j];
    
    sortIndices(points);
  }

  size_t query(const A& points, const Bounds& bounds, PointFunc f) const
  {
    auto lower_it = std::lower_bound(_indices.begin(), _indices.end(), bounds[0][0], [&points](index_t i, real value){
      return _x<1>(points[i]) < value;
    });

    auto upper_it = std::upper_bound(_indices.begin(), _indices.end(), bounds[1][0], [&points](real value, index_t i){
      return value < _x<1>(points[i]);
    });

    auto np = 0;

    for (auto it = lower_it; it != upper_it; ++it)
    {
      if (!f(points, *it)) break;
      np++;
    }

    return np;
  }

private:
  using real = typename P::value_type;
  IndexArray _indices;

  void sortIndices(const A& points)
  {
    auto cmpFunc = [&points](index_t i, index_t j){
      return points[i][0] < points[j][0];
    };
    std::sort(_indices.begin(), _indices.end(), cmpFunc);
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
    _indices = IndexArray(points.size());
    for (index_t i = 0; i < points.size(); ++i)
    {
      _indices[i] = i;
    }

    // sort the index array (canonic set of node) by their d-th coordinate
    sortIndices(points);
    build(_root, 0, _indices.size(), _indices, points);
  }

  void build(index_t first, index_t last, const IndexArray& indices, const A& points)
  {
    assert(!_root);
    _indices = IndexArray(last - first);

    for (auto i = 0u, j = first; i < last - first && j < last; i++, j++)
      _indices[i] = indices[j];

    // sorting by d-th coordinate and building
    sortIndices(points);
    build(_root, 0, _indices.size(), _indices, points);
  }

  size_t query(const A& points, const Bounds& bounds, PointFunc f) const
  {
    if(!_root) return 0;

    return query(_root, points, bounds, f);
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

    ~Node()
    {
      delete associatedTree;
      delete left;
      delete right;
    }

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

  Node* _root{};
  IndexArray _indices;

  void build (Node*& node, index_t first, index_t last, const IndexArray& indices, const A& points);
  
  size_t query(Node* node, const A& points, const Bounds& bounds, PointFunc f) const;
  void sortIndices(const A& points)
  {
    auto cmpFunc = [&points](index_t i, index_t j){
      return points[i][D - 1] < points[j][D - 1];
    };
    std::sort(_indices.begin(), _indices.end(), cmpFunc);
  }

}; // BBST

template<size_t D, typename P, typename A>
void BBST<D, P, A> :: build(Node*& node, index_t first, index_t last, const IndexArray& indices, const A& points)
{
  if (first == last)
    return;

  if (points[indices[first]][D - 1] != points[indices[last - 1]][D - 1])
  {
    // create a new node
    auto median = first + (last - first) / 2;
    node = new Node(points[indices[median]][D - 1], first, last);

    // searching new first and new last
    auto nf = median + 1;
    auto nl = median;
    for (; nf < last && points[_indices[nf]][D - 1] == points[_indices[median]][D - 1]; nf++);
    for (; nl > first && points[_indices[nl - 1]][D - 1] == points[_indices[median]][D - 1]; nl--);

    // recursive step
    build(node->left, first, nl, indices, points);
    build(node->right, nf, last, indices, points);
  }
  else
    node =  new Node(points[indices[first]][D - 1], first, last);

  // building associated trees
  node->associatedTree = new AssociatedTree();
  if (node != _root)
    node->associatedTree->build(node->_first, node->_last, indices, points);
  else
    node->associatedTree->build(points);
} // BBST::build

template<size_t D, typename P, typename A>
size_t BBST<D, P, A> :: query(Node* node, const A& points, const Bounds& bounds, PointFunc f) const
{
  if (!node) return 0;

  auto min_bounds = bounds[0][D-1];
  auto max_bounds = bounds[1][D-1];

  auto node_min = _x<D, P>(points[_indices[node->_first]]);
  auto node_max = _x<D, P>(points[_indices[node->_last-1]]);

  // Nó completamente contido na AABB.
  if(min_bounds <= node_min && max_bounds >= node_max)
  {
    if constexpr (D > 1)
    {
      if(node->associatedTree)
        return node->associatedTree->query(points, bounds, f);
    }
    else
    {
      size_t found = 0;

      for(index_t i = node->_first; i < node->_last; ++i)
      {
        if(!f(points, _indices[i])) break;
          found++;
      }

      return found;
    }
  }

  size_t np = 0;

  // Nó não completamente contido na AABB. Verifica os filhos.
  if (min_bounds <= node->_splitValue && node->left)
    np += query(node->left, points, bounds, f);
  
  if(max_bounds >= node->_splitValue && node->right)
    np += query(node->right, points, bounds, f);

  return np;
} // BBST::query


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
