#ifndef __Topology_h
#define __Topology_h

#include "../TriangleMesh.h"

/// @brief This is an example of a Topology, more specifically the given topology of P2.
namespace tcii::cg
{ // begin namespace tcii::cg

// meta programming to define what is a topological element?

using index_t = unsigned;

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

template<typename T, typename A>
class TopologyIteratorBase
{
public:
  void increment()
  {

  }

  void decrement()
  {

  }

  auto& operator++() // prefix increment
  {

  }

  auto operator++(int) // postfix increment
  {

  }

};

template<typename A>
concept is_topological_collection = requires(A collection, size_t i)
{
  // é indexável
  collection[i];
  // tem operação size()
  collection.size();
  // insert ACHO QUE PRECISA ?
  // remove ACHO QUE PRECISA ?
};



template<typename A>
requires is_topological_collection<A>
class Topology
{
public:
  using collection = A;

  Topology(TriangleMesh& mesh):
    
  {

  }

private:
  collection<Vertex> _vertices;
  collection<HalfEdge> _halfEdges;
  collection<Edge> _edges;
  collection<Face> _faces;
  collection<Boundary> _boundaries;
};

} // end namespace tcii::cg

#endif // __Topology_h