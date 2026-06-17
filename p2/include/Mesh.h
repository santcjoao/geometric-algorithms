#ifndef __Mesh_h
#define __Mesh_h

// OVERVIEW: Mesh.h
// ========
// Class definition for general mesh.
//
// Authors: Arthur de Andrade, João de Carvalho e Nathan Rezende
// Last revision: 17/06/2026

#include "TriangleMesh.h"
#include <functional>
#include <type_traits>
#include <concepts>

namespace tcii::cg
{ // begin namespace tcii::cg

using index_t = unsigned;

using ProcFunc = std::function<bool(size_t)>;

template<typename T>
concept is_topology = requires(T t, index_t i, index_t k, ProcFunc f)
{
  // basic topological elements
  typename T::collection;
  typename T::vert;
  typename T::edge;
  typename T::face;
  typename T::bounds;

  // deve ter um construtor que recebe um TriangleMesh
  requires std::constructible_from<T, TriangleMesh>;

  // templated functions
  t.template halfEdge<typename T::vert>(i);
  t.template halfEdge<typename T::face>(i);
  t.template halfEdge<typename T::bounds>(i);

  t.template incidentElement<typename T::edge>(i, f);
  t.template incidentElement<typename T::face>(i, f);
  
  t.template kRing<typename T::vert>(i, k, f);
  t.template kRing<typename T::face>(i, k, f);
  
  // basic functions
  { t.next(i) } -> std::same_as<index_t>;
  { t.twin(i) } -> std::same_as<index_t>;
  { t.hasFace(i) } -> std::same_as<bool>;
  { t.getVertex(i) } -> std::same_as<index_t>;
  { t.getEdge(i) } -> std::same_as<index_t>;
  { t.getFace(i) } -> std::same_as<index_t>;
  { t.distance(i, k) } -> std::same_as<float>;
};

template<typename T>
requires is_topology<T>
class Mesh
{
public:
  using topology = T;  
  using collection_type = T::collection;
  using vert = T::vert;
  using edge = T::edge;
  using face = T::face;
  using bounds = T::bounds;

  Mesh(TriangleMesh& mesh):
    _topology{mesh}
  {
    // do nothing
  }

  auto countIncidentFaces(const index_t& v) const
  {
    auto sum = 0;
    auto he = halfEdge<vert>(v);

    do
    {
      if(hasFace(he))
        sum++;

      he = next(twin(he));
    } while(he != halfEdge<vert>(v));
    return sum;
  }

  auto countIncidentEdges(const index_t& v) const
  {
    auto sum = 0;
    auto he = halfEdge<vert>(v);

    do
    {
      sum++;
      he = next(twin(he));
    } while(he != halfEdge<vert>(v));
    return sum;
  }

  auto perimeter(const index_t& boundary) const
  {
    auto len = 0.0f;
    auto he1 = halfEdge<bounds>(boundary);

    do
    {
      auto he2 = next(he1);
      len += distance(getVertex(he1), getVertex(he2));
      he1 = he2;
    } while(he1 != halfEdge<bounds>(boundary));
    return len;
  }

  template<typename E>
  auto halfEdge(const index_t& i) const
  {
    return _topology.halfEdge<E>(i);
  }
  
  auto next(const index_t& he) const
  {
    return _topology.next(he);
  }
  
  auto twin(const index_t& he) const
  {
    return _topology.twin(he);
  }
  
  auto hasFace(const index_t& he) const
  {
    return _topology.hasFace(he);
  }

  auto getVertex(const index_t& he) const
  {
    return _topology.getVertex(he);
  }

  auto getEdge(const index_t& he) const
  {
    return _topology.getEdge(he);
  }

  auto getFace(const index_t& he) const
  {
    return _topology.getFace(he);
  }

  auto distance(const index_t& e1, const index_t& e2) const
  {
    return _topology.distance(e1, e2);
  }

  template<typename E>
  void incidentElement(const index_t& e, ProcFunc f)
  {
    return _topology.incidentElement<E>(e, f);
  }

  template<typename E>
  void kRing(const index_t& e, size_t k, ProcFunc f)
  {
    return _topology.kRing<E>(e, k, f);
  }

private:
  topology _topology;
};

} // end namespace tcii::cg

#endif // __Mesh_h