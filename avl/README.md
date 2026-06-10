# Prova 1: Range Tree - Tópicos em Computação II 2026

## Autores

- Arthur Barbosa de Andrade
- João Pedro Santana de Carvalho
- Nathan de Almeida Rezende

## Status das Atividades

- Busca: Concluída
- Remoção: Concluída
- Iteração bidirecional: Concluída
- Iteração reversa: Concluída
- Teste de todas as funcionalidades de uma AVL: Concluída
- Gravação do Vídeo: Concluída

## Link do vídeo de explicação no Google Drive

INSERIR LINK

# Guia de Execução CLI

Este repositório disponibiliza alvos Makefile para executar testes e coletar métricas de desempenho.

## Comandos principais
- `make run` — Executa um teste único e imprime os resultados no console.
- `make profile` — Executa um intervalo iterativo de testes e exporta os dados para um arquivo CSV.
- `make profile-all` — Roda automaticamente a bateria assintótica completa (Pior, Normal e Melhor caso) iterando de 100k a 1M pontos.
- `make clean` — Exclui os artefatos de compilação e a pasta de benchmarks gerada.

## Argumentos de configuração
- `algo = naive | range | both`
    - `naive`: Executa estritamente a busca linear exaustiva.
    - `range`: Constrói a estrutura espacial e consulta apenas a Range Tree.
    - `both`: Executa ambos os algoritmos em sequência e valida a correspondência (integridade) dos resultados.

- `func = all | early | custom`
    - `all`: O predicado retorna verdadeiro para todos os pontos contidos na janela geométrica (coleta total).
    - `early`: O predicado retorna falso no primeiro ponto encontrado, acionando a interrupção prematura da busca (early exit).
    - `custom`: O predicado é dedicado para customização da função, passível de uso para testes específicos.

- `struct = array | vector`
    - `array`: Armazena os dados sintéticos utilizando o contêiner customizado `cg::Array`.
    - `vector`: Armazena os dados sintéticos utilizando a estrutura padrão `std::vector`.

## Métricas geradoras
- `dim = <int>` — Define a dimensionalidade do espaço vetorial gerado (2 ou 3).
- `count = <string>` — Quantidade total de pontos do conjunto estático (aceita notação k, ex: `150k`).
- `box = <float>` — Dimensão paramétrica da aresta da janela de consulta (AABB).
- `seed = <int>` — Semente de geração determinística para garantir a reprodutibilidade dos pontos no espaço.

## Parâmetros iterativos (exclusivos do `make profile`)
- `start = <string>` — Volume de pontos na primeira iteração (ex: `10k`).
- `end = <string>` — Volume de pontos na iteração final (ex: `100k`).
- `stride = <string>` — Incremento no volume de pontos a cada passo do loop (ex: `10k`).
- `out = <string>` — Nome do arquivo final exportado no diretório bruto (ex: `ensaio_2d.csv`).

## Exemplo de uso
```bash
make profile algo=both func=all struct=vector dim=2 count=100k box=0.1 seed=42 start=10k end=100k stride=10k out=ensaio_2d.csv
```