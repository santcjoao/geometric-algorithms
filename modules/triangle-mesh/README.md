# Half-Edge Triangle Mesh & Topological Pipeline (C++20)

Estrutura de dados topológica baseada no modelo **Half-Edge** (*Doubly-Connected Edge List* / DCEL) para representação, navegação e manipulação de superfícies poliédricas orientáveis de 2-variedades com bordas ($\text{2-manifolds with boundaries}$), integrada a uma arquitetura de pipeline desacoplada para anotação dinâmica de atributos (*mesh decoration*).

---

## 1. Fundamentos Topológicos e Invariantes

A estrutura Half-Edge decompõe cada aresta em duas semi-arestas orientadas opostas (*twins*), garantindo a navegação local em tempo constante $\mathcal{O}(1)$ entre vértices, faces e arestas incidentes:

$$\text{HalfEdge} = (v_{\text{target}}, f_{\text{incident}}, h_{\text{twin}}, h_{\text{next}}, h_{\text{prev}})$$

### Invariantes Preservados:
1. **Consistência de Ciclo de Face:** $\forall h, h.\text{next}.\text{next}.\text{next} = h$ em malhas triangulares.
2. **Involução de Gêmeos:** $\forall h, h.\text{twin}.\text{twin} = h$.
3. **Consistência de Fronteira:** Semi-arestas sem face associada são encadeadas em ciclos direcionados fechados ao longo do bordo da superfície ($\partial M$).
4. **Fórmula de Euler-Poincaré:** Validação da característica de Euler $\chi = V - E + F = 2 - 2g - b$, onde $g$ é o gênero topológico e $b$ é o número de componentes de borda.

---

## 2. Decisões de Arquitetura e Engenharia

* **Iteradores Circuladores:** Algoritmos de navegação local para consulta de vizinhanças $k$-ring (1-ring e 2-ring de vértices e faces) e bordas incidentes com alocação determinística.
* **Pipeline de Decoração Desacoplada:** Arquitetura orientada a estágios de processamento que anexa dados complementares (vetores normais de vértice/face, coordenadas de textura, campos de pesos escalares e cores) aos elementos da malha sem poluir a definição estrita da topologia conectiva.
* **Armazenamento Compacto (`FreeListVector` e SoA):** Alocador indexado com reciclagem de nós via lista encadeada interna para inserção e remoção de elementos sem fragmentação de memória.
* **Parser OBJ Wavefront:** Módulo nativo de decodificação de malhas poligonais em formato `.obj`, com construção robusta de semi-arestas e tratamento de arestas de fronteira.

---

## 3. Instruções de Compilação e Execução

### Compilação
```bash
make
```
Gera os arquivos objeto em `build/obj/` e o binário executável em `build/bin/p2_exec`.

### Execução
A execução requer a indicação do arquivo de malha Wavefront `.obj` via parâmetro `mesh`:

```bash
# Execução com malhas de teste inclusas no módulo:
make run mesh=meshes/tetra.obj
make run mesh=meshes/quad.obj

# Execução com malhas externas arbitrárias:
make run mesh=/caminho/para/modelo.obj
```

O programa instancia a representação Half-Edge, valida os invariantes topológicos, executa a verificação dos circuladores de vizinhança e processa as etapas da pipeline de anotação geométrica.

### Limpeza
```bash
make clean
```
