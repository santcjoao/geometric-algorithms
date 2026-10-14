# Generic AVL Tree (C++17)

Implementação genérica e estritamente tipada de uma Árvore AVL (*Self-Balancing Binary Search Tree*) desenvolvida em C++17, orientada a políticas de ordenação flexíveis e verificação estática de tipos.

---

## 1. Fundamentos Teóricos e Complexidade

A estrutura preserva o invariante estrito de balanceamento AVL: para qualquer nó $v \in T$, a diferença de altura entre suas subárvores esquerda e direita não excede uma unidade:

$$|h(\text{left}(v)) - h(\text{right}(v))| \le 1$$

O balanceamento é reestabelecido após inserções e deleções via rotações locais simples (LL, RR) ou duplas (LR, RL) em tempo $\mathcal{O}(1)$.

| Operação | Complexidade Temporal (Pior Caso) | Complexidade Espacial |
| :--- | :---: | :---: |
| **Busca (`find`, `contains`)** | $\mathcal{O}(\log n)$ | $\mathcal{O}(1)$ |
| **Inserção (`insert`)** | $\mathcal{O}(\log n)$ | $\mathcal{O}(1)$ amortizado |
| **Remoção (`erase`)** | $\mathcal{O}(\log n)$ | $\mathcal{O}(1)$ amortizado |
| **Percurso em Ordem (`iterator`)** | $\mathcal{O}(n)$ | $\mathcal{O}(1)$ adicional |

---

## 2. Decisões de Arquitetura e Engenharia

* **Policy-Based Design:** Ordenação parametrizada via *template functors* (`Compare`), aceitando desde operadores padrão (`std::less`, `std::greater`) até comparadores geométricos espaciais (e.g., ordenação léxica de vetores tridimensionais `Vec3<float>`).
* **Iteradores Bidirecionais:** Suporte a percursos em ordem (*in-order*) direta e reversa (`rbegin()`, `rend()`), compatíveis com *range-based for loops* e algoritmos da biblioteca padrão (`<algorithm>`).
* **Rastreamento de Ciclo de Vida e Memória:** Harness de teste instrumentado via classe `Traced`, que monitora a contagem de alocações/desalocações de nós para comprovar a ausência de vazamento de memória (*memory leaks*) e a correta aplicação de RAII durante rotações e destruição de subárvores.

---

## 3. Instruções de Compilação e Execução

O projeto inclui um `Makefile` determinístico com suporte a parametrização de testes via linha de comando.

### Compilação
```bash
make
```
Gera os arquivos objeto em `obj/` e o binário executável `avl`.

### Execução de Testes
```bash
# Executa a bateria de testes completa (todos os tipos e comparadores)
make run

# Execução customizada via parâmetros
make run type=int functor=less
make run type=vec functor=greater
make run type=traced
```

#### Parâmetros Suportados:
* `type`: Tipo de dado avaliado no harness (`int`, `vec`, `traced`, `all` — padrão: `all`).
* `functor`: Estratégia de ordenação (`less`, `greater`, `all` — padrão: `all`).

### Limpeza
```bash
make clean
```
