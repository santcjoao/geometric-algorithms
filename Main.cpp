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
#include <chrono>
#include <iostream>
#include <vector>

using namespace tcii::cg;

/// @brief Stage 1: Create initial decoration and color vertices
auto stage1 = [](const Mesh<Topology<FreeListVector>>& mesh) {

  if (mesh.getTopology().vertexCount() < 3)
    throw std::runtime_error("Degenerate mesh: insufficient vertices in the base geometry.");
  
  using Color = Vec3f;
  using VA = ElementSoA<Color>;
  using TD = TopologyDecoration<Mesh<Topology<FreeListVector>>, VA, void, void, void, void>;

  auto td = TD::New(mesh);
  set<VertexData, 0>(*td, 0, Color{1, 0, 0});
  return td;
};

/// @brief Stage 2: Add vertex normals based on the previous decoration
auto stage2 = [](auto previousDecoration) {
  using Color  = Vec3f;
  using Normal = Vec3f;

  using VA = ElementSoA<Color, Normal>;
  using TD = TopologyDecoration<Mesh<Topology<FreeListVector>>, VA, void, void, void, void>;
  
  auto td = TD::New(previousDecoration.get());
  set<VertexData, 1>(*td, 0, Normal{0, 0, 1});
  return td;
};

/// @brief Stage 3: Add weights to the edges
auto stage3 = [](auto previousDecoration) {
  using OldVA = typename decltype(previousDecoration)::element_type::VA;
  using EA = ElementSoA<float>;
  using TD = TopologyDecoration<Mesh<Topology<FreeListVector>>, OldVA, void, EA, void, void>;
  
  auto td = TD::New(previousDecoration.get());

  if (td->mesh()->topology().edgeCount() == 0)
    throw std::runtime_error("Unable to calculate weights: the mesh has no defined edges.");

  set<EdgeData, 0>(*td, 0, 1.5f);
  return td;
};

bool testMesh(const ObjectPtr<TriangleMesh>& mesh, const char* filename = "malha")
{
  if (!mesh)
  {
    std::cerr << "[ERROR] Could not read file: " << filename << '\n';
    return false;
  }

  if (mesh->data().vertexCount() == 0 || mesh->data().triangleCount() == 0)
  {
    std::cerr << "[ERROR] The read mesh is empty or corrupted.\n";
    return false;
  }

  return true;
}

int main()
{
  auto filename = "../../meshes/f-16.obj";
  auto triangleMesh = readOBJ(filename);

  if (!testMesh(triangleMesh, filename))
    return EXIT_FAILURE;

  try
  {
    std::cout << "[INFO] Building topology." << std::endl;
    auto t0 = std::chrono::steady_clock::now();

    Mesh<Topology<FreeListVector>> mesh(*triangleMesh);

    auto t1 = std::chrono::steady_clock::now();
    std::cout << "[INFO] Topology built in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
              << " ms" << std::endl;

    Pipeline renderPipeline(stage1, stage2);

    std::cout << "[INFO] Executing pipeline." << std::endl;
    auto t2 = std::chrono::steady_clock::now();

    auto finalDecoratedMesh = renderPipeline.execute(mesh);

    auto t3 = std::chrono::steady_clock::now();
    std::cout << "[INFO] Pipeline executed in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count()
              << " ms" << std::endl;

    std::cout << "[INFO] Pipeline executed successfully. Mesh ready for processing.\n";
  }
  catch (const std::exception& e)
  {
      std::cerr << "\n[ERROR] Interruption in the Decoration Pipeline stages.\n"
                << "Reason: " << e.what() << '\n';
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}