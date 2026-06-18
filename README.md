Trabalho 2 - Topologia e Decoração de Malhas
Disciplina: TCII
Autores: Arthur de Andrade, João de Carvalho e Nathan Rezende

Este projeto implementa uma estrutura de dados de malha topológica (Half-Edge) com iteradores const-correct bidirecionais e ferramentas de processamento de vizinhança (k-Ring e incidentElement), integrados a uma pipeline de Decoração de vértices, arestas e faces via SoA (Structure of Arrays).

## Funcionalidades e Requisitos Atendidos:
1. Construção da Malha: Lê arquivos `.obj`, converte os triângulos para representação baseada em Half-Edges e computa os limites topológicos (Boundaries), aceitando tanto malhas abertas (com buracos) quanto fechadas (sem bordas).
2. Iteradores: Iteradores funcionais e bidirecionais suportados para Vértices, Semi-Arestas, Arestas, Faces e Bordas.
3. Processamento de Vizinhança: Busca de elementos incidentes em constante O(grau) e varredura BFS para k-anéis de vértices ou faces através de `.incidentElement` e `.kRing`.
4. Decoração: Arquitetura em Pipeline permitindo que a Malha seja expandida em estágios. A Main instancia 3 estágios sucessivos inserindo Cores (Vértices), Normais (Vértices) e Pesos (Arestas).
5. Testes completos: A `Main.cpp` cobre ativamente todas as extrações de iteradores e navegações em 1-Ring e 2-Ring para demonstrar as estruturas operando integralmente na memória e a pipeline funcional.

## Como Compilar e Executar:

Sistemas Linux (Bash):

1. Limpar e Compilar o Projeto:
   make clean
   make

2. Rodar o Teste Padrão (utilizará o arquivo tetra.obj e quad.obj):
   make run

3. Rodar Testes Customizados (com as malhas da pasta meshes):
   make run mesh=meshes/quadra.obj
   make run mesh=meshes/f-16.obj
   make run mesh=meshes/monkey.obj

Ao executar, o console imprimirá o detalhamento da construção das variáveis Half-Edge e as impressões de teste do k-Ring, iteradores e as etapas da pipeline de Decoração.
