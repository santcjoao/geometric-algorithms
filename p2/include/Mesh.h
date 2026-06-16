#ifndef __Mesh_h
#define __Mesh_h

#include "TriangleMesh.h"
#include <functional>
#include <type_traits>
#include <concepts>

namespace tcii::cg
{ // begin namespace tcii::cg

using index_t = unsigned;

using ProcFunc = std::function<bool(size_t)>;

template<typename T>
concept is_topology = requires(T t, TriangleMesh m, index_t i, index_t j)
{
  // o que topologia deve ter de básico
  T::collection;
  T::vert;
  T::face;
  T::bounds;

  // deve ter um construtor que recebe um TriangleMesh
  requires std::constructible_from<T, TriangleMesh>;

  // funções de processamento
  t.halfEdge(i); // precisa explicitar que é <E>
  t.next(i);
  t.twin(i);
  t.hasFace(i);
  t.getVertex(i);
  t.getEdge(i);
  t.getFace(i);
  t.distance(i, j);
  // tem uma estrutura TopologyTraits<T> que define coisas sobre atributos topologicos
  // tem atributo templatizado com os elementos topológicos
  requires true; // bababui
};

template<typename T>
requires is_topology<T>
class Mesh
{
public:
  using topology = T;  
  using collection_type = T::collection;
  using vert = T::vert;
  using face = T::face;
  using bounds = T::bounds;

  Mesh(TriangleMesh& mesh):
    _topology{mesh}
  {
    // do nothing
  }

  // chamada de funções de processamento
  // incident faces (vert)
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

  // incident edges (vert)
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

  // perimeter (boundary)
  auto perimeter(const index_t& boundary) const
  {
    auto len = 0.0f;
    auto he1 = halfEdge<bounds>(boundary);

    do
    {
      auto he2 = halfEdge<bounds>(boundary);
      len += distance(getVertex(he1), getVertex(he2));
    } while(he1 != halfEdge<bounds>(boundary));
    return len;
  }

  // half edge (vert, boundary e faces)
  template<typename E>
  auto halfEdge(const index_t& i) const
  {
    return _topology.halfEdge<E>(i);
  }
  
  // next (half edge)
  auto next(const index_t& he) const
  {
    return _topology.next(he);
  }
  
  // twin (half edge)
  auto twin(const index_t& he) const
  {
    return _topology.twin(he);
  }
  
  // hasFace (half edge)
  auto hasFace(const index_t& he) const
  {
    return _topology.hasFace(he);
  }

  // vertex (he)
  auto getVertex(const index_t& he) const
  {
    return _topology.getVertex(he);
  }

  // edge (he)
  auto getEdge(const index_t& he) const
  {
    return _topology.getEdge(he);
  }

  // face ()
  auto getFace(const index_t& he) const
  {
    return _topology.getFace(he);
  }

  // distance (e1, e2)
  auto distance(const index_t& e1, const index_t& e2) const
  {
    return _topology.distance(e1, e2);
  }

  // template para incident faces
  template<typename E>
  incidentElement(const index_t& e, ProcFunc f)
  {
    return _topology.incidentElement<E>(e, f);
  }

  // template para k anel
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