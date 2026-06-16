#include "core/ParticleSystem.h"
#include "core/RangeTree.h"
#include "core/Types.h"
#include "include/graphics/Vec3.h"
#include <cstdio>
#include <iostream>
#include <random>
#include <vector>

using namespace tcii::physx;
using Vec3f = tcii::cg::Vec3f;

bool operator==(const Vec3f &a, const Vec3f &b) {
  return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

float randomFloat(float a, float b) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(a, b);
  return dist(gen);
}

Vec3f randomVec3f(const Vec3f &min, const Vec3f &max) {
  return Vec3f(randomFloat(min[0], max[0]), randomFloat(min[1], max[1]),
               randomFloat(min[2], max[2]));
}

int main() {
  // Create particle system with position, velocity, color
  auto ps = ParticleSystem<Vec3f, Vec3f>::New();
  ps->setParticleBuffer(1000);
  auto pb = ps->particles();

  Vec3f boxMin(-10, -10, -10);
  Vec3f boxMax(10, 10, 10);
  std::vector<Vec3f> colors = {Vec3f(1, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0, 1),
                               Vec3f(1, 1, 0)};

  // Adiciona partículas através do buffer, não do sistema
  for (int i = 0; i < 20; ++i) {
    Vec3f pos = randomVec3f(boxMin, boxMax);
    Vec3f vel = randomVec3f(Vec3f(-1, -1, -1), Vec3f(1, 1, 1));
    Vec3f col = colors[i % colors.size()];
    pb->add(pos, vel, col); // corrigido: pb->add
  }

  // Lista partículas usando iterador
  std::cout << "Particle list:\n";
  std::cout << "Index\tPosition\t\tVelocity\t\tColor\n";
  int idx = 0;
  for (auto it = pb->begin(); it != pb->end(); ++it) {
    const auto &pos = it.position();
    const auto &vel = it.get<1>(); // velocidade (primeiro field após posição)
    const auto &col = it.get<2>(); // cor
    std::cout << idx++ << "\t(" << pos[0] << ", " << pos[1] << ", " << pos[2]
              << ")\t"
              << "(" << vel[0] << ", " << vel[1] << ", " << vel[2] << ")\t"
              << "(" << col[0] << ", " << col[1] << ", " << col[2] << ")\n";
  }

  std::cout << "\nBounds: " << ps->bounds() << std::endl;

  // 1. Extrair as posições para um PointArray
  PointArray positions(pb->particleCount());
  for (unsigned i = 0; i < pb->particleCount(); ++i) {
    positions[i] = pb->position(i);
  }

  // 2. Construir a RangeTree (move semantics)
  RangeTreeArray rt(std::move(positions));
  rt.build();

  // 3. Definir a caixa de busca (ex: metade do espaço original)
  tcii::cg::Bounds3f queryBox;
  queryBox.inflate(Vec3f(-5, -5, -5)); // inflate com o ponto mínimo
  queryBox.inflate(Vec3f(5, 5, 5));    // inflate com o ponto máximo
  // Alternativa: criar bounds diretamente, mas Bounds3 não tem construtor com
  // dois pontos. Usamos inflate para expandir a partir do vazio.

  std::cout << "\nQuery box: " << queryBox << std::endl;

  // 4. Cor alvo (ex: vermelho)
  Vec3f targetColor(1, 0, 0);

  // 5. Função de filtro: conta as partículas que têm a cor alvo
  size_t matched = 0;
  auto colorFilter = [&](const PointArray &, size_t idx) -> bool {
    if (pb->get<2>(idx) == targetColor) // get<2> retorna a cor (terceiro campo)
      ++matched;
    return true; // sempre true para não interromper a busca
  };

  // 6. Executar a consulta
  size_t totalInBox = rt.query(queryBox, colorFilter);

  std::cout << "Total particles inside query box: " << totalInBox << std::endl;
  std::cout << "Particles with color " << targetColor
            << " inside box: " << matched << std::endl;

  // (Opcional) Validação manual
  size_t manualMatch = 0;
  for (unsigned i = 0; i < pb->particleCount(); ++i) {
    if (queryBox.contains(pb->position(i)) && pb->get<2>(i) == targetColor)
      ++manualMatch;
  }
  std::cout << "Manual validation: " << manualMatch << std::endl;

  puts("Press any key to exit...");
  (void)getchar();
  destroy(ps);
  return 0;
}