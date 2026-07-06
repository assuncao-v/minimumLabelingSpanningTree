# Minimum Labeling Spanning Tree Problem (MLSTP)

Trabalho Prático — DCC059 Teoria dos Grafos  
Universidade Federal de Juiz de Fora (UFJF)

**Equipe:** Rian César Quintanilha · Isaac Dizolele Kapela João · Vítor Assunção dos Santos · Welder Salvador Victor

---

## Sobre o Projeto

Implementação de uma heurística gulosa para o **Problema da Árvore Geradora com Número Mínimo de Rótulos (MLSTP)**.

Dado um grafo conexo onde cada aresta possui um rótulo, o objetivo é encontrar uma árvore geradora que conecte todos os vértices usando o menor número possível de rótulos distintos.

---

## Estrutura do Projeto

```
minimumLabelingSpanningTree/
├── include/
│   └── Graph.hpp                  # Definição das classes e structs
├── src/
│   ├── graph/
│   │   ├── graph.cpp              # Implementação do grafo e heurística MLST
│   │   └── main.cpp               # Lê as instâncias e executa o algoritmo
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
git clone https://github.com/isaac-kapela/trabalho-final-grafos
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
- `grafo_mvca` — executa a heurística nas instâncias

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

## Passo 4 — Executar a heurística

```bash
./build/bin/grafo_mvca
```

A saída mostra para cada instância o número de rótulos utilizados pela heurística:

```
Instancia: instancias/instancia01.txt  | vertices=100  arestas=990  rotulos=25  | Rotulos utilizados: 3
Instancia: instancias/instancia02.txt  | vertices=100  arestas=990  rotulos=25  | Rotulos utilizados: 4
...
```

---

## Como Testar

### Teste rápido (uma instância)

Verifique se a saída da instância 01 bate com o resultado esperado:

```bash
./build/bin/grafo_mvca 2>&1 | head -1
```

Saída esperada:
```
Instancia: instancias/instancia01.txt  | vertices=100  arestas=990  rotulos=25  | Rotulos utilizados: 3
```

### Teste completo (todas as instâncias)

```bash
./build/bin/grafo_mvca
```

Verifique se todas as 20 linhas aparecem sem erro e os rótulos utilizados batem com a tabela de resultados abaixo.

### Verificar reprodutibilidade

Como o gerador usa seed fixa, rodar `gerador_mlstp` duas vezes deve produzir arquivos idênticos:

```bash
cd instancias
../build/bin/gerador_mlstp
md5sum instancia01.txt   # anote o hash

../build/bin/gerador_mlstp
md5sum instancia01.txt   # deve ser igual ao anterior
```

---

## Resultados

| Instância | n | m | l | Rótulos usados |
|---|---|---|---|---|
| instancia01 | 100 | 990 | 25 | 3 |
| instancia02 | 100 | 990 | 25 | 4 |
| instancia03 | 100 | 2475 | 25 | 2 |
| instancia04 | 100 | 2475 | 25 | 2 |
| instancia05 | 100 | 3960 | 25 | 1 |
| instancia06 | 200 | 3980 | 50 | 5 |
| instancia07 | 200 | 3980 | 50 | 5 |
| instancia08 | 200 | 9950 | 50 | 2 |
| instancia09 | 200 | 9950 | 50 | 2 |
| instancia10 | 200 | 15920 | 50 | 2 |
| instancia11 | 500 | 24950 | 125 | 5 |
| instancia12 | 500 | 24950 | 125 | 5 |
| instancia13 | 500 | 62375 | 125 | 3 |
| instancia14 | 500 | 62375 | 125 | 3 |
| instancia15 | 500 | 99800 | 125 | 2 |
| instancia16 | 1000 | 99900 | 250 | 5 |
| instancia17 | 1000 | 99900 | 250 | 6 |
| instancia18 | 1000 | 249750 | 250 | 3 |
| instancia19 | 1000 | 249750 | 250 | 3 |
| instancia20 | 1000 | 399600 | 250 | 2 |

---

## Observação

Quanto maior a densidade do grafo, menor o número de rótulos necessários — com mais arestas disponíveis, a heurística consegue conectar todos os vértices com menos rótulos distintos.
