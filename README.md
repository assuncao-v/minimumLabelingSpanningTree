# Minimum Labeling Spanning Tree Problem (MLSTP)

Trabalho Prático — DCC059 Teoria dos Grafos  
Universidade Federal de Juiz de Fora (UFJF)

**Equipe:** Rian César Quintanilha · Isaac Dizolele Kapela João · Vítor Assunção dos Santos · Welder Salvador Victor

---

## Sobre o Projeto

Implementação de três algoritmos para o **Problema da Árvore Geradora com Número Mínimo de Rótulos (MLSTP)**:

- **MVCA** — heurística gulosa determinística
- **MVCA Randomizado** — GRASP com alpha fixo, 100 iterações por chamada
- **GRASP Reativo** — GRASP com alpha dinâmico, 200 iterações por chamada

---

## Estrutura do Projeto

```
minimumLabelingSpanningTree/
├── include/
│   └── Graph.hpp                  # Definição das classes e structs
├── src/
│   ├── graph/
│   │   ├── graph.cpp              # Implementação do grafo e algoritmos
│   │   └── main.cpp               # Experimentos 1, 2 e 3
│   ├── generator/
│   │   └── generator.cpp          # Gerador das 20 instâncias de teste
│   └── disjoint-set-union/
│       ├── UnionFind.h
│       └── UnionFind.cpp          # Union-Find (detecção de ciclos)
├── instancias/                    # Arquivos .txt gerados
├── CMakeLists.txt
└── README.md
```

---

## Pré-requisitos

- CMake >= 3.16
- Compilador C++17 (g++ ou clang++)

---

## Passo 1 — Clonar o repositório

```bash
git clone https://github.com/assuncao-v/minimumLabelingSpanningTree
cd minimumLabelingSpanningTree
```

---

## Passo 2 — Compilar o projeto

```bash
cmake -S . -B build
cmake --build build
```

Os binários ficam em `build/bin/`:
- `gerador_mlstp` — gera as instâncias
- `grafo_mvca` — executa os três experimentos

---

## Passo 3 — Gerar as instâncias

```bash
mkdir -p instancias
cd instancias
../build/bin/gerador_mlstp
cd ..
```

Serão criados 20 arquivos `instancia01.txt` ... `instancia20.txt` dentro da pasta `instancias/`.

### Formato do arquivo de instância

```
n m l
u1 v1 rotulo1
u2 v2 rotulo2
...
```

| Campo | Significado |
|---|---|
| `n` | número de vértices |
| `m` | número de arestas |
| `l` | número de rótulos disponíveis |
| `u v rotulo` | aresta entre os vértices u e v com o rótulo atribuído |

---

## Passo 4 — Rodar os algoritmos

```bash
./build/bin/grafo_mvca
```

O programa executa três experimentos em sequência:

### Experimento 1 — MVCA Guloso
Roda o MVCA uma vez em cada instância. Exibe o número de rótulos utilizados e o tempo de execução.

```
============================================================
  EXPERIMENTO 1 — MVCA Guloso (1 execucao por instancia)
============================================================

Instancia         Rotulos     Tempo (ms)
--------------------------------------------
instancia01       4           0.386
instancia02       4           0.283
...
```

### Experimento 2 — MVCA Randomizado
Para cada instância e cada valor de alpha `{0.05, 0.10, 0.15, 0.30, 0.50}`, executa o MVCA randomizado **30 vezes**. Exibe o resultado de cada execução e a média de rótulos e tempo por alpha.

```
============================================================
  EXPERIMENTO 2 — MVCA Randomizado
  30 execucoes por alpha | alphas: {0.05, 0.10, 0.15, 0.30, 0.50}
============================================================
...
  Media (alpha=0.10): rotulos=3.00  tempo=21.906ms
```

### Experimento 3 — GRASP Reativo
Executa o GRASP Reativo **30 vezes** por instância. O alpha é ajustado dinamicamente a cada bloco de 20 iterações com base no desempenho histórico. Exibe ao final duas tabelas: qualidade e tempo.

```
============================================================
  EXPERIMENTO 3 — GRASP Reativo (30 execucoes por instancia)
============================================================
...
  TABELA DE QUALIDADE — GRASP Reativo
  TABELA DE TEMPO — GRASP Reativo
```

### Salvar saída em arquivo

```bash
./build/bin/grafo_mvca > resultados.txt
```

---

## Como Testar

### Teste rápido (Experimento 1 apenas)

```bash
./build/bin/grafo_mvca 2>&1 | head -25
```

### Verificar reprodutibilidade das instâncias

```bash
cd instancias
../build/bin/gerador_mlstp
md5sum instancia01.txt   # anote o hash

../build/bin/gerador_mlstp
md5sum instancia01.txt   # deve ser igual ao anterior
```

---

## Observação

Quanto maior a densidade do grafo, menor o número de rótulos necessários. O MVCA Randomizado com alphas pequenos (0.05, 0.10) tende a encontrar soluções melhores que o MVCA puro, enquanto alphas maiores aumentam a exploração mas podem piorar a qualidade média.
