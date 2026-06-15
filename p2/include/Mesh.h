#ifndef __Mesh_h
#define __Mesh_h

#include "TriangleMesh.h"
#include <type_traits>
#include <concepts>

namespace tcii::cg
{ // begin namespace tcii::cg

template<typename T>
concept is_topology = requires(T t, TriangleMesh& m)
{
  // define tais elementos
  T::collection;
  
  // deve ter um construtor que recebe um TriangleMesh
  t(m);
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

  Mesh(TriangleMesh& mesh):
    _topology{mesh}
  {
    // do nothing
  }

  // chamada de funções de processamento
  // incident edges
  // k-ring (vert)
  // k-ring (face)

private:
  topology _topology;
};

} // end namespace tcii::cg

#endif // __Mesh_h