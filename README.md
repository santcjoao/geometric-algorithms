# TCII - Trabalho 2
**Autores:** Arthur de Andrade, João de Carvalho e Nathan Rezende

Este código implementa uma estrutura de malha Half-Edge com ferramentas de busca de vizinhança, integrada a um sistema de pipeline para adicionar dados extras (decoração) em vértices, arestas e faces.

## Funcionalidades
- **Construção da Malha:** Lê arquivos `.obj` e cria a representação Half-Edge, suportando malhas fechadas e com buracos.
- **Iteradores:** Permitem percorrer os elementos da malha (vértices, faces, arestas e bordas).
- **Busca de Vizinhança:** Algoritmos para encontrar elementos adjacentes.
- **Decoração em Pipeline:** Processa e anexa propriedades à malha em etapas. A demonstração atual inclui a adição de Cores, Normais e Pesos.

## Compilação e Execução

Requer um ambiente Linux.

**1. Limpar compilação:**
```bash
make clean
```

**2. Compilar o projeto:**
```bash
make
```

**3. Executar:**
É obrigatório informar o arquivo `.obj` que será carregado usando o parâmetro `mesh`. Exemplos:
```bash
make run mesh=meshes/tetra.obj
make run mesh=meshes/quad.obj
make run mesh=meshes/monkey.obj
```

O programa testará as funções da malha, incluindo iteradores e vizinhanças, além de processar as etapas da pipeline de decoração, exibindo os resultados no console.
