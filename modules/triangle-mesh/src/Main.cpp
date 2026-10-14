// OVERVIEW: Main.cpp
// ========
// Main function for the topology experiment.
//
// Authors: Arthur de Andrade, João de Carvalho e Nathan Rezende
// Last revision: 17/06/2026

#include "graphics/Topology.h"
#include "Mesh.h"
#include "MeshDecoration.h"
#include "Pipeline.h"
#include "TriangleMesh.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>

using namespace tcii::cg;

using MyTopology = Topology<FreeListVector>;
using MyMesh = Mesh<MyTopology>;

// Logging
void section(std::string_view title) {
  std::cout << "\n--- " << title << " ---\n";
}

void logMeshInfo(std::string_view name, size_t vertexCount, size_t faceCount) {
  std::cout << "  Loaded " << name << " (" << vertexCount << " vertices, "
             << faceCount << " faces).\n";
}

// Small helpers 
/// Counts the elements in an iterator range.
template <typename Iterator>
size_t countRange(Iterator begin, Iterator end) {
  return static_cast<size_t>(std::distance(begin, end));
}

// Pipeline Stages
/// Stage 1: Create initial decoration and color vertices
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

/// Stage 2: Add vertex normals based on the previous decoration
auto stage2 = [](auto previousDecoration) {
  using Color  = Vec3f;
  using Normal = Vec3f;

  using VA = ElementSoA<Color, Normal>;
  using TD = TopologyDecoration<MyMesh, VA, void, void, void, void>;

  auto td = TD::New(previousDecoration.get());
  set<VertexData, 1>(*td, 0, Normal{0, 0, 1});
  return td;
};

/// Stage 3: Add weights to the edges
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

// Tests 
void testTopologyFunctionalities(MyMesh& mesh) {
  auto& topo = mesh.getTopology();

  std::cout << "  Testing iterators:\n";

  // 1. Iterator tests: Verify if we can iterate over all topological entities
  const size_t vCount  = countRange(topo.begin<MyTopology::vert>(),   topo.end<MyTopology::vert>());
  const size_t heCount = countRange(topo.begin<MyTopology::he>(),     topo.end<MyTopology::he>());
  const size_t eCount  = countRange(topo.begin<MyTopology::edge>(),   topo.end<MyTopology::edge>());
  const size_t fCount  = countRange(topo.begin<MyTopology::face>(),   topo.end<MyTopology::face>());
  const size_t bCount  = countRange(topo.begin<MyTopology::bounds>(), topo.end<MyTopology::bounds>());

  std::cout << "    Iterated over: " << vCount << "v, " << heCount << "he, "
             << eCount << "e, " << fCount << "f, " << bCount << "b.\n";

  std::cout << "  Testing neighborhood (Vertex 0):\n";
  // 2. Neighborhood queries: Extract incident elements (O(degree)) and perform BFS (k-Rings)
  std::cout << "    Incident edges: ";
  topo.incidentElement<MyTopology::edge>(0, [](size_t id) {
    std::cout << id << ' ';
    return true;
  });
  std::cout << '\n';

  std::cout << "    Incident faces: ";
  topo.incidentElement<MyTopology::face>(0, [](size_t id) {
    std::cout << id << ' ';
    return true;
  });
  std::cout << '\n';

  std::cout << "    1-ring faces: ";
  topo.kRing<MyTopology::vert>(0, 1, [](size_t id) {
    std::cout << id << ' ';
    return true;
  });
  std::cout << '\n';

  std::cout << "    2-ring faces: ";
  topo.kRing<MyTopology::vert>(0, 2, [](size_t id) {
    std::cout << id << ' ';
    return true;
  });
  std::cout << '\n';

  if (fCount > 0) {
    std::cout << "  Testing neighborhood (Face 0):\n";
    // 3. Face adjacency: Test 1-ring neighbors for a face
    std::cout << "    1-ring faces: ";
    topo.kRing<MyTopology::face>(0, 1, [](size_t id) {
      std::cout << id << ' ';
      return true;
    });
    std::cout << '\n';
  }
}

void runTests(const char* filename, std::string_view label) {
  section(label);

  // Parse the Wavefront OBJ file and build a basic geometry container
  auto triangleMesh = readOBJ(filename);
  if (!triangleMesh || triangleMesh->data().vertexCount() == 0) {
    std::cerr << "  [ERROR] Could not read or invalid file: " << filename << '\n';
    return;
  }

  logMeshInfo(filename, triangleMesh->data().vertexCount(), triangleMesh->data().triangleCount());

  try {
    std::cout << "  [INFO] Building topology.\n";
    const auto t0 = std::chrono::steady_clock::now();
    MyMesh mesh(*triangleMesh);
    const auto t1 = std::chrono::steady_clock::now();
    std::cout << "  [INFO] Topology built in "
               << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
               << " ms\n";

    testTopologyFunctionalities(mesh);

    std::cout << "  [INFO] Executing pipeline with 3 stages.\n";
    // Construct the rendering pipeline: Color (v) -> Normal (v) -> Weight (e)
    Pipeline renderPipeline(stage1, stage2, stage3);

    const auto t2 = std::chrono::steady_clock::now();
    [[maybe_unused]] auto finalDecoratedMesh = renderPipeline.execute(mesh);
    const auto t3 = std::chrono::steady_clock::now();

    std::cout << "  [INFO] Pipeline executed in "
               << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count()
               << " ms\n";
    std::cout << "  [INFO] Pipeline executed successfully. Mesh ready.\n";

  } catch (const std::exception& e) {
    std::cerr << "  [ERROR] Execution interrupted.\n"
               << "  Reason: " << e.what() << '\n';
  }
}

// Main
int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <mesh.obj>\n";
    return EXIT_FAILURE;
  }

  const std::string title = "Mesh Test: " + std::string(argv[1]);
  runTests(argv[1], title);

  return EXIT_SUCCESS;
}