# 3D Orthogonal Range Tree & Particle Spatial Querying (C++17)

Estrutura de dados hierárquica multidimensional voltada para consultas espaciais ortogonais em $\mathbb{R}^3$, integrada a um subsistema de partículas com múltiplos atributos físicos.

---

## 1. Fundamentos Teóricos e Complexidade

A **Range Tree** decompõe o espaço tridimensional recursivamente em uma árvore primária associada a árvores secundárias e terciárias para cada dimensão ortogonal $(X, Y, Z)$:

* **Construção:** Ordenação e subdivisão em cada eixo em tempo $\mathcal{O}(n \log^{d-1} n)$ para dimensão $d = 3$, resultando em complexidade $\mathcal{O}(n \log^2 n)$.
* **Consulta por Janela (AABB - *Axis-Aligned Bounding Box*):** Dada uma caixa delimitadora $[\mathbf{p}_{\min}, \mathbf{p}_{\max}]$, a estrutura recupera os $k$ pontos contidos no intervalo em tempo:

$$\mathcal{O}(\log^d n + k) = \mathcal{O}(\log^3 n + k)$$

* **Consumo Espacial:** $\mathcal{O}(n \log^{d-1} n) = \mathcal{O}(n \log^2 n)$.

---

## 2. Decisões de Arquitetura e Engenharia

* **Semântica de Movimentação (*Move Semantics* & Zero-Copy):** O construtor e o operador de atribuição por movimentação (`RangeTree(RangeTree&&)` e `operator=(RangeTree&&)`) transferem diretamente a custódia das tabelas internas de coordenadas sem clonar vetores, garantindo transferência de *ownership* em $\mathcal{O}(1)$.
* **Separação Indexação vs. Payload:** O índice espacial opera sobre projeções geométricas, enquanto o buffer de partículas desacoplado (`ParticleBuffer`) mantém os atributos físicos (posição, velocidade e codificação de cor).
* **Filtragem Composta por Predicados:** Suporte à composição de consultas geométricas de janela (AABB) com predicados booleanos customizados sobre os atributos dos dados (e.g., particionar e isolar partículas contidas na região espacial que satisfaçam condições cromáticas específicas em tempo constante por ponto recuperado).

---

## 3. Instruções de Compilação e Execução

### Compilação
```bash
make
```
Gera os arquivos objeto e o binário executável em `build/main.exe`.

### Execução
```bash
make run
```
Gera uma nuvem sintética de partículas em $\mathbb{R}^3$, constrói o índice geométrico via movimentação de *buffers*, executa a consulta espacial por AABB com predicado de atributo e valida a saída contra uma busca linear de referência (*ground truth*).

### Limpeza
```bash
make clean
```