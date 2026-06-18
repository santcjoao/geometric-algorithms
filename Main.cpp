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

using MyTopology = Topology<FreeListVector>;
using MyMesh = Mesh<MyTopology>;

// ---- Logging --------------------------------------------------------------

void section(const char *title) { 
  std::cout << "\n--- " << title << " ---\n"; 
}

void logMeshInfo(const char* name, size_t v, size_t f) {
  std::cout << "  Loaded " << name << " (" << v << " vertices, " << f << " faces).\n";
}

// ---- Pipeline Stages ------------------------------------------------------

/// @brief Stage 1: Create initial decoration and color vertices
auto stage1 = [](const MyMesh& mesh) {

  if (mesh.getTopology().vertexCount() < 3)
    throw std::runtime_error("Degenerate mesh: insufficient vertices in the base geometry.");
  
  using Color = Vec3f;
  using VA = ElementSoA<Color>;
  using TD = TopologyDecoration<MyMesh, VA, void, void, void, void>;

  auto td = TD::New(mesh);
  set<VertexData, 0>(*td, 0, Color{1, 0, 0});
  return td;
};

/// @brief Stage 2: Add vertex normals based on the previous decoration
auto stage2 = [](auto previousDecoration) {
  using Color  = Vec3f;
  using Normal = Vec3f;

  using VA = ElementSoA<Color, Normal>;
  using TD = TopologyDecoration<MyMesh, VA, void, void, void, void>;
  
  auto td = TD::New(previousDecoration.get());
  set<VertexData, 1>(*td, 0, Normal{0, 0, 1});
  return td;
};

/// @brief Stage 3: Add weights to the edges
auto stage3 = [](auto previousDecoration) {
  using OldVA = typename decltype(previousDecoration)::element_type::VA;
  using EA = ElementSoA<float>;
  using TD = TopologyDecoration<MyMesh, OldVA, void, EA, void, void>;
  
  auto td = TD::New(previousDecoration.get());

  if (td->mesh()->getTopology().edgeCount() == 0)
    throw std::runtime_error("Unable to calculate weights: the mesh has no defined edges.");

  set<EdgeData, 0>(*td, 0, 1.5f);
  return td;
};

// ---- Tests ----------------------------------------------------------------

void testTopologyFunctionalities(MyMesh& mesh) {
  auto& topo = mesh.getTopology();
  
  std::cout << "  Testing iterators:\n";
  size_t vCount = 0;
  for (auto it = topo.begin<MyTopology::vert>(); it != topo.end<MyTopology::vert>(); ++it) vCount++;
  
  size_t eCount = 0;
  for (auto it = topo.begin<MyTopology::edge>(); it != topo.end<MyTopology::edge>(); ++it) eCount++;

  size_t fCount = 0;
  for (auto it = topo.begin<MyTopology::face>(); it != topo.end<MyTopology::face>(); ++it) fCount++;

  size_t bCount = 0;
  for (auto it = topo.begin<MyTopology::bounds>(); it != topo.end<MyTopology::bounds>(); ++it) bCount++;

  std::cout << "    Iterated over: " << vCount << "v, " << eCount << "e, " << fCount << "f, " << bCount << "b.\n";

  std::cout << "  Testing neighborhood (Vertex 0):\n";
  std::cout << "    Incident edges: ";
  topo.incidentElement<MyTopology::edge>(0, [](size_t edgeID) {
      std::cout << edgeID << " ";
      return true;
  });
  std::cout << "\n";

  std::cout << "    1-ring faces: ";
  topo.kRing<MyTopology::vert>(0, 1, [](size_t faceID) {
      std::cout << faceID << " ";
      return true;
  });
  std::cout << "\n";

  if (fCount > 0) {
    std::cout << "  Testing neighborhood (Face 0):\n";
    std::cout << "    1-ring faces: ";
    topo.kRing<MyTopology::face>(0, 1, [](size_t faceID) {
        std::cout << faceID << " ";
        return true;
    });
    std::cout << "\n";
  }
}

void runTests(const char* filename, const char* label) {
  section(label);

  auto triangleMesh = readOBJ(filename);
  if (!triangleMesh || triangleMesh->data().vertexCount() == 0) {
    std::cerr << "  [ERROR] Could not read or invalid file: " << filename << '\n';
    return;
  }

  logMeshInfo(filename, triangleMesh->data().vertexCount(), triangleMesh->data().triangleCount());

  try {
    std::cout << "  [INFO] Building topology.\n";
    auto t0 = std::chrono::steady_clock::now();
    MyMesh mesh(*triangleMesh);
    auto t1 = std::chrono::steady_clock::now();
    std::cout << "  [INFO] Topology built in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
              << " ms\n";

    testTopologyFunctionalities(mesh);

    std::cout << "  [INFO] Executing pipeline with 3 stages.\n";
    Pipeline renderPipeline(stage1, stage2, stage3);
    
    auto t2 = std::chrono::steady_clock::now();
    auto finalDecoratedMesh = renderPipeline.execute(mesh);
    auto t3 = std::chrono::steady_clock::now();
    
    std::cout << "  [INFO] Pipeline executed in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count()
              << " ms\n";
    std::cout << "  [INFO] Pipeline executed successfully. Mesh ready.\n";

  } catch (const std::exception& e) {
      std::cerr << "  [ERROR] Execution interrupted.\n"
                << "  Reason: " << e.what() << '\n';
  }
}

// ---- Main -----------------------------------------------------------------

int main(int argc, char** argv) {
  if (argc > 1) {
    std::string title = std::string("Custom Mesh Test: ") + argv[1];
    runTests(argv[1], title.c_str());
  } else {
    // Test with closed mesh (no boundaries)
    runTests("meshes/tetra.obj", "1. Closed Mesh Test");

    // Test with open mesh (with boundaries)
    runTests("meshes/quad.obj", "2. Open Mesh Test");
  }

  return EXIT_SUCCESS;
}