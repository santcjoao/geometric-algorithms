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

// ---- Utilitários ----------------------------------------------------------

bool operator==(const Vec3f &a, const Vec3f &b) {
  return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

float randomFloat(float a, float b) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return std::uniform_real_distribution<float>(a, b)(gen);
}

Vec3f randomVec3f(const Vec3f &lo, const Vec3f &hi) {
  return {randomFloat(lo[0], hi[0]), randomFloat(lo[1], hi[1]),
          randomFloat(lo[2], hi[2])};
}

char colorTag(const Vec3f &c) {
  if (c[0] == 1 && c[1] == 0 && c[2] == 0)
    return 'R';
  if (c[0] == 0 && c[1] == 1 && c[2] == 0)
    return 'G';
  if (c[0] == 0 && c[1] == 0 && c[2] == 1)
    return 'B';
  if (c[0] == 1 && c[1] == 1 && c[2] == 0)
    return 'Y';
  return '?';
}

// ---- Logging --------------------------------------------------------------

void section(const char *title) { std::cout << "\n--- " << title << " ---\n"; }

void logMoveBefore(const char *action, const char *srcName, size_t srcSize) {
  std::cout << "  Antes do " << action << ":\n"
            << "    " << srcName << ".size() = " << srcSize << "\n";
}

void logMoveAfter(const char *action, const char *srcName, size_t srcSize,
                  const char *dstName, size_t dstSize) {
  std::cout << "  Depois do " << action << ":\n"
            << "    " << srcName << ".size() = " << srcSize << "\n"
            << "    " << dstName << ".size() = " << dstSize << "\n";
}

void logParticleTable(ParticleBuffer<Vec3f, Vec3f> *pb,
                      const tcii::cg::Bounds3f &queryBox) {
  std::cout
      << "\n #   Posicao              Velocidade           | COR  CONTIDO\n";
  int i = 0;
  for (auto it = pb->begin(); it != pb->end(); ++it, ++i) {
    const auto &p = it.position();
    const auto &v = it.get<1>();
    const auto &c = it.get<2>();
    printf(" %-3d (%6.2f,%6.2f,%6.2f)  (%6.2f,%6.2f,%6.2f)  |  %c      %d\n", i,
           p[0], p[1], p[2], v[0], v[1], v[2], colorTag(c),
           queryBox.contains(p) ? 1 : 0);
  }
}

void logQueryResults(size_t rtTotal, size_t rtRed, size_t bfTotal,
                     size_t bfRed) {
  printf("  %-24s RangeTree  Manual\n", "");
  printf("  Total na AABB:          %-10zu %zu\n", rtTotal, bfTotal);
  printf("  Vermelhas na AABB:      %-10zu %zu\n", rtRed, bfRed);
  bool ok = (rtTotal == bfTotal && rtRed == bfRed);
  std::cout << "  Resultado: " << (ok ? "OK" : "FALHA") << "\n";
}

// ---- Testes ---------------------------------------------------------------

// Popula o ParticleSystem com N partículas aleatórias.
void populateParticles(ParticleBuffer<Vec3f, Vec3f> *pb, int n) {
  Vec3f lo(-10, -10, -10), hi(10, 10, 10);
  Vec3f colors[] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 0}};

  for (int i = 0; i < n; ++i)
    pb->add(randomVec3f(lo, hi), randomVec3f({-1, -1, -1}, {1, 1, 1}),
            colors[i % 4]);
}

// Constrói a RangeTree usando move semantics e imprime prova.
RangeTreeArray buildTreeWithMove(ParticleBuffer<Vec3f, Vec3f> *pb) {
  // Extrai posições do buffer
  PointArray pos(pb->particleCount());
  for (unsigned i = 0; i < pb->particleCount(); ++i)
    pos[i] = pb->position(i);

  // Move constructor
  logMoveBefore("move constructor", "positions", pos.size());
  RangeTreeArray tmp(std::move(pos));
  tmp.build();
  logMoveAfter("move constructor", "positions", pos.size(), "tempRt.points()",
               tmp.points().size());
  std::cout << "\n";

  // Move assignment
  logMoveBefore("move assignment", "tempRt.points()", tmp.points().size());
  RangeTreeArray rt;
  rt = std::move(tmp);
  logMoveAfter("move assignment", "tempRt.points()", tmp.points().size(),
               "rt.points()", rt.points().size());

  return rt;
}

// Executa consulta AABB com e sem filtro, retorna contagens via parâmetros
void runQuery(RangeTreeArray &rt, ParticleBuffer<Vec3f, Vec3f> *pb,
              const tcii::cg::Bounds3f &box, size_t &rtTotal, size_t &rtRed,
              size_t &bfTotal, size_t &bfRed) {
  Vec3f red{1, 0, 0};

  // RangeTree: sem filtro
  rtTotal = rt.query(box, [](const PointArray &, size_t) { return true; });

  // RangeTree: filtro por cor vermelha
  rtRed = rt.query(box, [&](const PointArray &, size_t i) -> bool {
    return pb->get<2>(i) == red;
  });

  // Brute-force: validação
  bfTotal = bfRed = 0;
  for (unsigned i = 0; i < pb->particleCount(); ++i) {
    if (box.contains(pb->position(i))) {
      ++bfTotal;
      if (pb->get<2>(i) == red)
        ++bfRed;
    }
  }
}

// ---- Main -----------------------------------------------------------------

int main() {
  constexpr int N = 20;

  // 1. Construção multi-atributo
  section("1. Construcao do ParticleSystem (3D, multi-atributo)");
  auto ps = ParticleSystem<Vec3f, Vec3f>::New();
  ps->setParticleBuffer(1000);
  auto pb = ps->particles();
  populateParticles(pb, N);

  tcii::cg::Bounds3f queryBox;
  queryBox.inflate(Vec3f(-5, -5, -5));
  queryBox.inflate(Vec3f(5, 5, 5));

  std::cout << N << " particulas geradas.\n";
  std::cout << "Bounds do sistema: " << ps->bounds() << "\n";
  std::cout << "Regiao de busca:   " << queryBox << "\n";
  logParticleTable(pb, queryBox);

  // 2. Move semantics
  section("2. Construcao da RangeTree (move semantics)");
  RangeTreeArray rt = buildTreeWithMove(pb);

  // 3. Consulta AABB + predicado
  section("3. Consulta AABB com predicado customizado");
  size_t rtTotal, rtRed, bfTotal, bfRed;
  runQuery(rt, pb, queryBox, rtTotal, rtRed, bfTotal, bfRed);
  logQueryResults(rtTotal, rtRed, bfTotal, bfRed);

  // Limpeza
  puts("\nPressione ENTER para sair...");
  (void)getchar();
  destroy(ps);
  return 0;
}