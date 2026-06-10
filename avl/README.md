# AVL Tree

Implementação de uma Árvore AVL genérica em C++17.

## Guia de Uso (Makefile)

### Compilar

```bash
make
```

Compila todo o código-fonte em `src/` e gera o executável `avl`.

### Executar

```bash
make run
```

Compila (se necessário) e executa todos os testes com os valores padrão (`type=all`, `functor=all`).

#### Parâmetros

A execução aceita dois parâmetros opcionais:

| Parâmetro | Valores aceitos                  | Padrão |
|-----------|----------------------------------|--------|
| `type`    | `all`, `int`, `vec`, `traced`    | `all`  |
| `functor` | `all`, `less`, `greater`         | `all`  |

- **`type`** — seleciona qual tipo de dado testar:
  - `int` — testes com inteiros.
  - `vec` — testes com `Vec3<float>`.
  - `traced` — testes com a classe `Traced`.
  - `all` — executa todos os tipos acima.

- **`functor`** — seleciona o comparador utilizado:
  - `less` — ordem crescente.
  - `greater` — ordem decrescente.
  - `all` — executa ambos.

#### Exemplos

```bash
# Executa todos os testes
make run

# Testa apenas inteiros com ambos os comparadores
make run type=int

# Testa apenas Vec3 em ordem decrescente
make run type=vec functor=greater

# Testa apenas a classe Traced em ordem crescente
make run type=traced functor=less

# Testa todos os tipos, mas apenas com comparador greater
make run functor=greater
```

### Limpar

```bash
make clean
```

Remove o executável `avl` e o diretório `obj/` com os arquivos objeto.
