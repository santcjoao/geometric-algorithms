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
  // deve ter um construtor que recebe um TriangleMesh
  // tem uma estrutura TopologyTraits<T> que define coisas sobre atributos topologicos
  // tem atributo templatizado com os elementos topológicos
};

template<typename A>
concept is_collection_of_topological_elements = requires(A collection, size_t i)
{
  // é indexável
  // tem operação size()
};

template<typename T, typename A = typename std::vector> // não sei se ta certo se pa que n mas é por ai
requires is_topology<T> && is_collection_of_topological_elements<A>
class Mesh
{
public:
  using Topology = T<A>;  

  Mesh(TriangleMesh& mesh):
    _topology{mesh}
  {
    // do nothing
  }

private:
  Topology _topology;
};

} // end namespace tcii::cg

#endif // __Mesh_h