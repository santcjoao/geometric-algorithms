#ifndef __MeshDecoration_h
#define __MeshDecoration_h

// OVERVIEW: MeshDecoration.h
// ========
// Class definition for triangle mesh decoration.
//
// Author: Paulo Pagliosa
// Last revision: 03/06/2026
// 
// ========
// 
// Class definition for general mesh decoration.
//
// Authors: Arthur de Andrade, João de Carvalho e Nathan Rezende
// Last revision: 16/06/2026

#include "DecorationSet.h"
#include "TriangleMesh.h"

namespace tcii::cg
{ // begin namespace tcii::cg


/////////////////////////////////////////////////////////////////////
//
// MeshDecoration: class for triangle mesh decoration
// ==============
template <typename V, typename T>
class EMPTY_BASES MeshDecoration: public SharedObject,
  public DecorationSet<V, T>
{
public:
  using VA = V;
  using TA = T;
  using Base = DecorationSet<V, T>;
  using pointer = ObjectPtr<MeshDecoration>;

  static auto New(const TriangleMesh& mesh)
  {
    return pointer{new MeshDecoration{&mesh}};
  }

  template <typename OtherV, typename OtherT>
  static auto New(MeshDecoration<OtherV, OtherT>* other)
  {
    assert(other != nullptr);
    return pointer{new MeshDecoration{std::move(*other)}};
  }

  TriangleMesh* mesh() const
  {
    return _mesh;
  }

  TriangleMesh* mesh()
  {
    return _mesh;
  }

private:
  ObjectPtr<TriangleMesh> _mesh;

  MeshDecoration(const TriangleMesh* mesh):
    Base{mesh->data().vertexCount(), mesh->data().triangleCount()},
    _mesh{mesh}
  {
    // do nothing
  }

  MeshDecoration(MeshDecoration&&) noexcept = default;

  template <typename OtherV, typename OtherT>
  MeshDecoration(MeshDecoration<OtherV, OtherT>&& other):
    Base{std::move(other)},
    _mesh{other.mesh()}
  {
    // do nothing
  }

}; // MeshDecoration for triangle meshes

/// @brief Mesh decoration for general topologies
/// @tparam TopoMeshType Type of the Mesh (e.g., Mesh<Topology<std::vector>>)
template <typename TopoMeshType, typename V, typename HE, typename E, typename F, typename B>
class EMPTY_BASES TopologyDecoration:
  public SharedObject,
  public DecorationSet<V, HE, E, F, B>
{
public:
  using VA  = V;
  using HEA = HE;
  using EA  = E;
  using FA  = F;
  using BA  = B;

  using Base = DecorationSet<V, HE, E, F, B>;
  using pointer = ObjectPtr<TopologyDecoration>;

  static auto New(const TopoMeshType& mesh)
  {
    return pointer{new TopologyDecoration{&mesh}};
  }

  template <typename OtherV, typename OtherHE, typename OtherE, typename OtherF, typename OtherB>
  static auto New(TopologyDecoration<TopoMeshType,OtherV, OtherHE, OtherE, OtherF, OtherB>* other)
  {
    assert(other != nullptr);
    return pointer{new TopologyDecoration{std::move(*other)}};
  }

  TopoMeshType* mesh() const { return _mesh;}
  TopoMeshType* mesh()       { return _mesh;}

private:
  ObjectPtr<TopoMeshType> _mesh;

  DecorationSet(const Mesh<TopoMeshType>* mesh):
    Base{
      mesh->topology().vertexCount(),
      mesh->topology().halfEdgeCount(),
      mesh->topology().edgeCount(),
      mesh->topology().faceCount(),
      mesh->topology().boundaryCount()
    },
    _mesh{mesh}
  { /* Do nothing */ }

  DecorationSet(Base&&) noexcept = default;

  template<typename OtherV, typename OtherHE, typename OtherE, typename OtherF, typename OtherB>
  DecorationSet(TopologyDecoration<OtherV, OtherHE, OtherE, OtherF, OtherB>&& other):
    Base{std::move(other)},
    _mesh{other.mesh()}
  { /* Do nothing */ }

}; // TopologyDecoration

} // end namespace tcii::cg

#endif // __MeshDecoration_h