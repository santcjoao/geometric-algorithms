#ifndef __RangeTree_h
#define __RangeTree_h

#include "include/util/Array.h"
#include "PointTraits.h"
#include <functional>
#include <numeric>
#include <type_traits>
#include <concepts>

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

  BBST() = default;

  BBST(BBST&& other) noexcept:
    _indices{std::move(other._indices)}
  {
    // do nothing
  }

  BBST& operator =(BBST&& other) noexcept
  {
    if (this != &other)
    {
      this->~BBST(); // Isso gera erro aparentemente.
      _indices = std::move(other._indices); 
    }
    return *this;
  }

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
    size_t np = 0;

    query(points, bounds, f, np);

    return np;
  }

  void query(const A& points, const Bounds& bounds, PointFunc f, size_t& np) const
  {

    auto lower_it = std::lower_bound(_indices.begin(), _indices.end(), bounds[0][0], [&points](index_t i, real value){
      return _x<1>(points[i]) < value;
    });

    auto upper_it = std::upper_bound(_indices.begin(), _indices.end(), bounds[1][0], [&points](real value, index_t i){
      return value < _x<1>(points[i]);
    });

    for (auto it = lower_it; it != upper_it; ++it)
    {
      if (!f(points, *it)) continue;
      np++;
    }
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

  BBST() = default;

  BBST(BBST&& other) noexcept:
    _root{std::exchange(other._root, nullptr)},
    _indices{std::move(other._indices)}
  {
    // do nothing
  }

  BBST& operator =(BBST&& other) noexcept
  {
    if (this != &other)
    {
      this->~BBST(); // ERRO?
      _root = std::exchange(other._root, nullptr);
      _indices = std::move(other._indices); 
    }
    return *this;
  }

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

    size_t np = 0;

    query(_root, points, bounds, f, np);

    return np;
  }

  void query(const A& points, const Bounds& bounds, PointFunc f, size_t& np) const
  {
    if(!_root) return;
    query(_root, points, bounds, f, np);
  }

private:
  using real = typename P::value_type;
  using AssociatedTree = BBST<D - 1, P, A>;

  struct Node
  {
    real _splitValue; // Pivô. Valor da d-ésima coordenada do ponto

    // Intervalo [first, last) de pontos associados a este conjunto canônico
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
  
  void query(Node* node, const A& points, const Bounds& bounds, PointFunc f, size_t& np) const;

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
  node->associatedTree->build(node->_first, node->_last, indices, points);
} // BBST::build

template<size_t D, typename P, typename A>
void BBST<D, P, A> :: query(Node* node, const A& points, const Bounds& bounds, PointFunc f, size_t& np) const
{
  if (!node) return;

  auto min_bounds = bounds[0][D-1];
  auto max_bounds = bounds[1][D-1];

  auto node_min = _x<D, P>(points[_indices[node->_first]]);
  auto node_max = _x<D, P>(points[_indices[node->_last-1]]);

  // Conjunto canônico completamente contido na AABB
  if(min_bounds <= node_min && max_bounds >= node_max)
  {
    if(node->associatedTree)
      node->associatedTree->query(points, bounds, f, np);
    
    return;
  }

  // Conjunto nodal contido na AABB
  if (min_bounds <= node->_splitValue || max_bounds >= node->_splitValue)
  {
    auto median = node->_first + (node->_last - node->_first) / 2;

    // Verificação dos pontos posteriores à mediana
    for(index_t i = median + 1; i < node->_last && _x<D, P>(points[_indices[i]]) == node->_splitValue; ++i)
    {
      if(bounds.contains(points[_indices[i]]))
      {
        if(!f(points, _indices[i])) continue;
        np ++;
      }
    }

    // Verificação dos pontos anteriores à mediana e da própria mediana
    for (index_t i = median + 1; i > node->_first; )
    {
      --i;
      
      if (_x<D, P>(points[_indices[i]]) != node->_splitValue) break;

      if (bounds.contains(points[_indices[i]]))
      {
        if (!f(points, _indices[i])) continue;
        np++;
      }
    }
  }

  // Verificação dos filhos
  if (min_bounds <= node->_splitValue && node->left)
    query(node->left, points, bounds, f, np);
  
  if(max_bounds >= node->_splitValue && node->right)
    query(node->right, points, bounds, f, np);
} // BBST::query

} // end namespace rtree

template <typename P>
concept is_point = requires(P p, size_t i)
{
  typename P::value_type;
  requires std::is_floating_point_v<typename P::value_type>;
  { p[i] } -> std::convertible_to<typename P::value_type>;
  PointTraits<P>::dim;
  typename PointTraits<P>::Bounds;
  requires point_dim_v<P> == PointTraits<P>::dim;
};

template <typename P, typename A>
concept is_point_collection = requires(A collection, size_t i)
{
  { collection[i] } -> std::convertible_to<P>;
  collection.size();
  requires std::movable<A>; // Garantee move semantic for performance
};

template <typename P, typename A>
requires is_point<P> && is_point_collection<P, A>
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

  RangeTree(RangeTree&& other) noexcept:
    _points{std::move(other._points)},
    _mainTree{std::move(other._mainTree)}
  {
    // do nothing
  }

  RangeTree& operator =(RangeTree&& other) noexcept
  {
    if (this != &other)
    {
      this->~RangeTree(); // ERRO ?
      _points = std::move(other._points);
      _mainTree = std::move(other._mainTree);
    }
    return *this;
  }

  ~RangeTree() = default;

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
  const A _points;
  rtree::BBST<D, P, A> _mainTree;

}; // RangeTree

} // end namespace tcii::cg

#endif // __RangeTree_h