#include "core/ParticleSystem.h"
#include "core/RangeTree.h"
#include "core/Types.h"
#include "graphics/Vec3.h"
#include <cstdio>
#include <iostream>
#include <random>
#include <vector>

using namespace tcii::physx;
using Vec3f = tcii::cg::Vec3f;

// Comparação componente-a-componente de Vec3f
bool operator==(const Vec3f &a, const Vec3f &b) {
  return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

// Geração de float aleatório no intervalo [a, b]
float randomFloat(float a, float b) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(a, b);
  return dist(gen);
}

// Geração de Vec3f aleatório dentro de uma bounding box [min, max]
Vec3f randomVec3f(const Vec3f &min, const Vec3f &max) {
  return Vec3f(randomFloat(min[0], max[0]), randomFloat(min[1], max[1]),
               randomFloat(min[2], max[2]));
}

// Separador visual para o console
void printSeparator(const char *title) {
  std::cout << "\n--- " << title << " ---\n";
}

int main() {
  constexpr int NUM_PARTICLES = 20;

  // 1. CONSTRUÇÃO 3D MULTI-ATRIBUTO
  printSeparator("1. Construcao do ParticleSystem (3D, multi-atributo)");

  auto ps = ParticleSystem<Vec3f, Vec3f>::New();
  ps->setParticleBuffer(1000);
  auto pb = ps->particles();

  // Limites do mundo e paleta de cores
  Vec3f worldMin(-10, -10, -10);
  Vec3f worldMax(10, 10, 10);
  Vec3f red(1, 0, 0), green(0, 1, 0), blue(0, 0, 1), yellow(1, 1, 0);
  std::vector<Vec3f> palette = {red, green, blue, yellow};

  // Popula o buffer com partículas aleatórias
  for (int i = 0; i < NUM_PARTICLES; ++i) {
    Vec3f pos = randomVec3f(worldMin, worldMax);
    Vec3f vel = randomVec3f(Vec3f(-1, -1, -1), Vec3f(1, 1, 1));
    Vec3f col = palette[i % palette.size()];
    pb->add(pos, vel, col);
  }

  std::cout << NUM_PARTICLES << " particulas geradas.\n";
  std::cout << "Bounds do sistema: " << ps->bounds() << "\n";

  // Define a região de busca (AABB) antecipadamente para usar na listagem
  tcii::cg::Bounds3f queryBox;
  queryBox.inflate(Vec3f(-5, -5, -5));
  queryBox.inflate(Vec3f(5, 5, 5));

  // Brute-force: verifica containment de cada partícula na AABB
  std::vector<bool> contained(pb->particleCount());
  for (unsigned i = 0; i < pb->particleCount(); ++i)
    contained[i] = queryBox.contains(pb->position(i));

  // Inicial da cor (R/G/B/Y)
  auto colorTag = [&](const Vec3f &c) -> char {
    if (c == red)
      return 'R';
    if (c == green)
      return 'G';
    if (c == blue)
      return 'B';
    if (c == yellow)
      return 'Y';
    return '?';
  };

  // Listagem das partículas com info de cor e containment
  std::cout
      << "\n #   Posicao              Velocidade           | COR  CONTIDO\n";
  int idx = 0;
  for (auto it = pb->begin(); it != pb->end(); ++it) {
    const auto &pos = it.position();
    const auto &vel = it.get<1>();
    const auto &col = it.get<2>();
    printf(" %-3d (%6.2f,%6.2f,%6.2f)  (%6.2f,%6.2f,%6.2f)  |  %c      %d\n",
           idx, pos[0], pos[1], pos[2], vel[0], vel[1], vel[2], colorTag(col),
           contained[idx] ? 1 : 0);
    ++idx;
  }

  // 2. MOVE SEMANTICS
  printSeparator("2. Construcao da RangeTree (move semantics)");

  // Copia posições do buffer para um PointArray independente
  PointArray positions(pb->particleCount());
  for (unsigned i = 0; i < pb->particleCount(); ++i)
    positions[i] = pb->position(i);

  std::cout << "-> Antes do move constructor:\n";
  std::cout << "   positions.size() = " << positions.size() << "\n";

  // Move constructor: transfere ownership do array para a árvore temporária
  RangeTreeArray tempRt(std::move(positions));
  tempRt.build();
  std::cout << "-> Depois do move constructor:\n";
  std::cout << "   positions.size() = " << positions.size() << "\n";
  std::cout << "   tempRt.points().size() = " << tempRt.points().size()
            << "\n\n";

  std::cout << "-> Antes do move assignment:\n";
  std::cout << "   tempRt.points().size() = " << tempRt.points().size() << "\n";

  // Move assignment: transfere a árvore inteira para outra variável
  RangeTreeArray rt;
  rt = std::move(tempRt);
  std::cout << "-> Depois do move assignment:\n";
  std::cout << "   tempRt.points().size() = " << tempRt.points().size() << "\n";
  std::cout << "   rt.points().size() = " << rt.points().size() << "\n";

  // 3. CONSULTA ESPACIAL (AABB) + FILTRAGEM POR PREDICADO
  printSeparator("3. Consulta AABB com predicado customizado");
  std::cout << "Regiao de busca: " << queryBox << "\n";

  // Consulta sem filtro: conta todas as partículas na AABB
  size_t totalInBox =
      rt.query(queryBox, [](const PointArray &, size_t) { return true; });

  // Consulta com filtro: apenas partículas vermelhas na AABB
  Vec3f targetColor = red;
  auto redFilter = [&](const PointArray &, size_t i) -> bool {
    return pb->get<2>(i) == targetColor;
  };
  size_t redInBox = rt.query(queryBox, redFilter);

  std::cout << "Particulas na AABB (RangeTree):   " << totalInBox << "\n";
  std::cout << "Vermelhas na AABB (RangeTree):     " << redInBox << "\n";

  // Compara com brute-force já calculado
  size_t manualTotal = 0, manualRed = 0;
  for (unsigned i = 0; i < pb->particleCount(); ++i) {
    if (contained[i]) {
      ++manualTotal;
      if (pb->get<2>(i) == targetColor)
        ++manualRed;
    }
  }

  std::cout << "Particulas na AABB (manual):      " << manualTotal << "\n";
  std::cout << "Vermelhas na AABB (manual):        " << manualRed << "\n";

  bool ok = (totalInBox == manualTotal && redInBox == manualRed);
  std::cout << "Resultado: " << (ok ? "OK" : "FALHA") << "\n";

  // Limpeza
  puts("\nPressione ENTER para sair...");
  (void)getchar();
  destroy(ps);
  return 0;
}