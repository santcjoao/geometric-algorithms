# Integração: Range Tree e Particle System - Tópicos em Computação II 2026

## Autores

- Arthur Barbosa de Andrade
- João Pedro Sant' Ana de Carvalho
- Nathan de Almeida Rezende

## Sobre o Projeto

Este projeto demonstra a integração e extensão da estrutura de dados **Range Tree** para atuar em conjunto com um sistema físico de partículas (`ParticleSystem`). O foco dos testes presentes na `main` é validar a utilização da Range Tree para consultas espaciais tridimensionais (AABB) em buffers de partículas contendo múltiplos atributos (como Posição, Velocidade e Cor).

## Link do vídeo no drive

https://drive.google.com/file/d/1y8ri2dJg5MruWDgbdxQjctFW9An2S7oi/view?usp=drive_link

## Principais Funcionalidades Testadas

- **Construção 3D Multi-atributo**: Construção de uma Range Tree tridimensional aceitando tuplas de dados complexos (Partículas estruturadas através de variados *Fields* via *Templates*).
- **Semântica de Movimentação (Move Semantics)**: Validação da transferência de propriedade dos dados (*ownership transfer*) na inicialização da árvore, garantindo a performance da estrutura ao não copiar dados grandes.
- **Consulta Espacial e Filtragem**: Consulta em janela (AABB - Axis-Aligned Bounding Box) em conjunto com predicados customizados (ex: buscar espacialmente e filtrar apenas as partículas contidas na AABB que sejam puramente da cor vermelha `(1, 0, 0)`).

## Guia de Execução

O repositório disponibiliza targets simples através do `Makefile` para facilitar a compilação e o teste do programa. 

### Comandos:
- `make` — Compila todo o código-fonte (processando a `main` e classes utilitárias/físicas associadas) e gera o executável no diretório `/build`.
- `make run` — Executa o programa principal. O console imprimirá o status e a lista inicial de partículas espalhadas pelo mundo, para em seguida exibir as saídas validadas do teste de intersecção/busca através da *Range Tree*.
- `make clean` — Limpa completamente os artefatos de build gerados durante a compilação.