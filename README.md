# Advanced Data Structures & Geometric Algorithms

Implementações formais em C++ Moderno (C++17/C++20) de estruturas de dados avançadas para busca balanceada, indexação espacial multidimensional e topologia computacional de superfícies poliédricas.

---

## 1. Visão Geral e Estrutura dos Módulos

O repositório está organizado como um monorepo modular e ortogonal, contendo três subsistemas independentes:

```text
TCII-2026/
├── modules/
│   ├── avl-tree/          # Árvore AVL genérica baseada em políticas (C++17)
│   ├── range-tree/        # Range Tree 3D com move semantics e busca por AABB (C++17)
│   └── triangle-mesh/     # Malha Half-Edge e pipeline de decoração geométrica (C++20)
├── Makefile               # Orquestrador unificado de compilação e testes
└── README.md              # Documentação central do projeto
```

### Módulos do Sistema:

1. [**Generic AVL Tree (`modules/avl-tree`)**](modules/avl-tree/README.md)
   * Árvore binária de busca auto-balanceada com invariant de altura $|h_L - h_R| \le 1$.
   * *Policy-based design* com comparadores customizáveis (`std::less`, `std::greater`, `Vec3Less`).
   * Iteradores bidirecionais (direto e reverso) e instrumentação estática de memória com detecção de vazamentos.

2. [**3D Orthogonal Range Tree (`modules/range-tree`)**](modules/range-tree/README.md)
   * Árvore de busca multidimensional ortogonal decomposta em três níveis para $\mathbb{R}^3$.
   * Complexidade de consulta por janela AABB em $\mathcal{O}(\log^3 n + k)$.
   * Semântica de movimentação (*move semantics*) com transferência de custódia zero-copy e composição de predicados de atributos sobre buffers de partículas.

3. [**Half-Edge Polygonal Mesh (`modules/triangle-mesh`)**](modules/triangle-mesh/README.md)
   * Estrutura Half-Edge (*Doubly-Connected Edge List*) para representação de 2-variedades orientáveis com borda.
   * Circuladores de vizinhança topológica (1-ring e 2-ring para vértices e faces) em tempo local $\mathcal{O}(1)$.
   * Pipeline extensível de decoração geométrica (normais, pesos escalares e propriedades cromáticas) desacoplada da topologia de incidência.

---

## 2. Co-autoria e Matriz de Responsabilidade

Projeto desenvolvido em colaboração para a disciplina de Tópicos em Computação II (UFMS):

| Autor | Responsabilidade Técnica Principal |
| :--- | :--- |
| **João Pedro Sant'Ana de Carvalho** ([@santcjoao](https://github.com/santcjoao)) | Engenharia de compilação determinística (`Makefiles` modulares e orquestrador raiz), modularização monorepo, instrumentação da suíte de testes parametrizados da Árvore AVL (suporte a tipos genéricos e verificação de RAII/vazamento de memória) e framework de testes e logging de invariantes topológicos da Half-Edge. |
| **Arthur Barbosa** ([@shiro-sama404](https://github.com/shiro-sama404)) | Estruturas topológicas da Half-Edge, encadeamento de semi-arestas e alocador indexado `FreeListVector`; lógica de busca (`find`), remoção (`erase`) e rotações de balanceamento na Árvore AVL; integração do subsistema de partículas (`ParticleBuffer`) na Range Tree. |
| **Nathan Rezende** ([@nahnn](https://github.com/nahnn)) | Algoritmos de consulta de vizinhança $k$-ring (1-ring e 2-ring) e circuladores na Half-Edge; implementação de *Move Semantics* (construtor e operador de movimentação) e restrições com C++ *Concepts* na Range Tree; iteradores bidirecionais e reversos na Árvore AVL. |

---

## 3. Guia Rápido de Compilação e Execução

O orquestrador na raiz permite construir e testar todos os módulos a partir de um único ponto de entrada.

### Compilar Todos os Módulos
```bash
make all
```

### Executar os Testes dos Módulos
```bash
# Executa a suíte de testes da Árvore AVL
make test-avl

# Executa o benchmark e verificação da Range Tree 3D
make test-rt

# Executa a instanciação e validação da Half-Edge Mesh (utiliza meshes/tetra.obj)
make test-mesh
```

### Limpar Todos os Artefatos de Build
```bash
make clean
```
