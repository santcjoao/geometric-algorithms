#ifndef __Topology_h
#define __Topology_h

#include "../TriangleMesh.h"

/// @brief This is an example of a Topology, more specifically the given topology of P2.
namespace tcii::cg
{ // begin namespace tcii::cg

// meta programming to define what is a topological element?

using index_t = unsigned;
using TM = TriangleMesh;

struct Vertex
{
  index_t id;
  index_t heID;
  Vec3f pos;
};

struct HalfEdge
{
  index_t vID;
  index_t twin;
  index_t eID;
  index_t fID;
  index_t next;
  index_t prev;
};

struct Edge
{
  index_t id;
  index_t twins[2];
};

struct Face
{
  index_t id;
  index_t heID;
};

struct Boundary
{
  index_t id;
  index_t heID;
};

// template<typename A> class Topology;

template<typename T, typename A> // talvez seja independente de A?
class TopologyIteratorBase
{
public:
  using collection_type = A;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using difference_type = std::ptrdiff_t;

  void increment()
  {

  }

  void decrement()
  {

  }

  bool operator ==()
  {

  }

  bool operator !=()
  {

  }

  auto& operator*() const { return ((TreeNode<T> *)_node)->_value; }

  auto operator->() const { return &operator*(); }

  auto& operator ++() // prefix increment
  {

  }

  auto operator ++(int) // postfix increment
  {

  }

  auto& operator --() // prefix increment
  {

  }

  auto operator --(int) // postfix increment
  {

  }

private:
  T _value;  
};

template<typename A>
concept is_topological_collection = requires(A collection, index_t i)
{
  // é indexável -> tem uma ordem
  collection[i];
  // tem operação size()
  { collection.size() } -> std::integral;
  
  // next pro iterador se pa? mas entra na brisa de ter

  // não pode construtor a partir de tais elementos pq 
  // insert ACHO QUE PRECISA ? Só que pra elementos topológicos
  // remove ACHO QUE PRECISA ?
};



template<typename A>
requires is_topological_collection<A>
class Topology
{
public:
  using collection = A;
  using vert = Vertex;
  using he = HalfEdge;
  using edge = Edge;
  using face = Face;
  using bounds = Boundary;

  Topology(TriangleMesh& mesh):
    _vertices{},
    _halfEdges{},
    _edges{},
    _faces{},
    _boundaries{}
  {
    for(auto i = 0u; i < mesh.data().vertices().size(); ++i)
      _vertices.push_back(mesh.data().vertices()[i]);
    
    for(auto i = 0u; i < mesh.data().triangles().size(); ++i)
      _faces.push_back(mesh.data().triangles()[i]);
    
    // TODO: Construir he, e, b e dar uma ordem pra todos os elem top.
  }

  template<typename E>
  auto halfEdge(const index_t i) const
  {
    if constexpr (E == vert)
      return _vertices[i].heID;
    else if constexpr (E == face)
      return _faces[i].heID;
    else if constexpr (E == bounds)
      return _boundaries[i].heID;
    else
      return -1; // n sei se da
  }

  // next (half edge)
  auto next(const index_t& he) const
  {
  }
  
  // twin (half edge)
  auto twin(const index_t& he) const
  {
  }
  
  // hasFace (half edge)
  auto hasFace(const index_t& he) const
  {
  }

  // vertex (he)
  auto getVertex(const index_t& he) const
  {
  }

  // edge (he)
  auto getEdge(const index_t& he) const
  {
  }

  // face ()
  auto getFace(const index_t& he) const
  {
  }

  // distance (e1, e2)
  auto distance(const index_t& e1, const index_t& e2) const
  {
    
  }

  template<typename E>
  incidentElement(const index_t& e, ProcFunc f)
  {
    if constexpr (E == edge)
    {
      auto he = halfEdge<vert>(v);

      do
      {
        auto e = getEdge(he);
        f(e);
        he = next(twin(he));
      } while(he != halfEdge<vert>(v));
    }
    else if constexpr (E == face)
    {
      auto he = halfEdge<vert>(v);

      do
      {
        if (hasFace(he))
        {
          auto fc = getFace(he);
          f(fc);
        }

        he = next(twin(he));
      } while(he != halfEdge<vert>(v));
    }
    else
      return;
  }

  // template para k anel
  template<typename E>
  void kRing(const index_t& e, size_t k, ProcFunc f)
  {
    if constexpr (E == vert)
    else if constexpr (E == face)
    else
      return;
  }

private:
  collection<vert> _vertices;
  collection<he> _halfEdges;
  collection<edge> _edges;
  collection<face> _faces;
  collection<bounds> _boundaries;
};

} // end namespace tcii::cg

#endif // __Topology_h