// OVERVIEW: Main.cpp
// ========
// Main function for the topology experiment.
//
// Authors: Arthur de Andrade, João de Carvalho e Nathan Rezende
// Last revision: 16/06/2026

#include "graphics/Topology.h"
#include "Mesh.h"
#include "MeshDecoration.h"
#include "Pipeline.h"
#include "TriangleMesh.h"
#include <iostream>
#include <vector>

using namespace tcii::cg;

/// @brief Stage 1: Create initial decoration and color vertices
auto stage1 = [](const Mesh<Topology<std::vector>>& mesh) {

  if (mesh.topology().vertexCount() < 3)
    throw std::runtime_error("Malha degenerada: Vértices insuficientes na geometria base.");
  
  using Color = Vec3f;
  using VA = ElementSoA<Color>;
  using TD = TopologyDecoration<Mesh<Topology<std::vector>>, VA, void, void, void, void>;

  auto td = TD::New(mesh);
  set<VD, 0>(*td, 0, Color{1, 0, 0});
  return td;
};

/// @brief Stage 2: Add vertex normals based on the previous decoration
auto stage2 = [](auto previousDecoration) {
  using Color  = Vec3f;
  using Normal = Vec3f;

  using OldVA = typename decltype(previousDecoration)::element_type::VA;
  using VA = ElementSoA<Color, Normal>;
  using TD = TopologyDecoration<Mesh<Topology<std::vector>>, VA, void, void, void, void>;
  
  auto td = TD::New(previousDecoration.get());
  set<VD, 1>(*td, 0, Normal{0, 0, 1});
  return td;
};

/// @brief Stage 3: Add weights to the edges
auto stage3 = [](auto previousDecoration) {
  using OldVA = typename decltype(previousDecoration)::element_type::VA;
  using EA = ElementSoA<float>;
  using TD = TopologyDecoration<Mesh<Topology<std::vector>>, OldVA, void, EA, void, void>;
  
  auto td = TD::New(previousDecoration.get());

  if (td->mesh()->topology().edgeCount() == 0)
    throw std::runtime_error("Impossível calcular pesos: a malha não possui arestas definidas.");

  set<ED, 0>(*td, 0, 1.5f);
  return td;
};

bool testMesh(const TriangleMesh& mesh, const char* filename = "malha")
{
  if (!mesh)
  {
    std::cerr << "[Erro] Não foi possível ler o arquivo: " << filename << '\n';
    return false;
  }

  if (mesh->data().vertexCount() == 0 || mesh->data().triangleCount() == 0)
  {
    std::cerr << "[Erro] A malha lida está vazia ou corrompida.\n";
    return false;
  }

  return true;
}

int main()
{
  auto filename = "../../meshes/f-16.obj";
  auto triangleMesh = readOBJ(filename);

  if (!testMesh(*triangleMesh, filename))
    return EXIT_FAILURE;

  try
  {
    Mesh<Topology<std::vector>> mesh(*triangleMesh);

    Pipeline renderPipeline(stage1, stage2);

    auto finalDecoratedMesh = renderPipeline.execute(mesh);

    std::cout << "Pipeline executado com sucesso! Malha pronta para processamento.\n";
  }
  catch (const std::exception& e)
  {
      std::cerr << "\n[Falha] Interrupção nos estágios do Pipeline de Decoração.\n"
                << "Motivo: " << e.what() << '\n';
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}