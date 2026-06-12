#ifndef __Mesh_h
#define __Mesh_h

#include "TriangleMesh.h"
#include <type_traits>
#include <concepts>

namespace tcii::cg
{ // begin namespace tcii::cg

template<typename T>
concept is_topology = requires(T t)
{
  // define tais elementos
  // tem uma estrutura TopologyTraits<T> que define coisas sobre atributos topologicos
  // tem atributo templatizado com os elementos topológicos
};

template<typename A>
concept is_collection_of_topological_elements = requires(size_t i)
{
  // é indexável
  // tem operação size()
};

template<typename T, typename A>
requires is_topology<T> && is_collection_of_topological_elements<A>
class Mesh
{
public:
  
  Mesh(TriangleMesh& mesh):
  {
    // do nothing
  }

private:
  T _topology;
};

} // end namespace tcii::cg

#endif // __Mesh_h