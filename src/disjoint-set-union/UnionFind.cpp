#include "UnionFind.h"
#include <iostream>
#include <vector>
#include <string>
#include <numeric>


UnionFind::UnionFind(int n) {
    parent.resize(n);
    // Inicializa cada vértice como seu próprio pai
    std::iota(parent.begin(), parent.end(), 0);
    // Inicializa o rank de todos como 0
    rank.assign(n, 0);
}

// Função Find com Compactação de Caminho (Path Compression)
int UnionFind::find(int i) {
    if (parent[i] == i) {
        return i;
    }
    return parent[i] = find(parent[i]); // Altera o pai direto para a raiz do conjunto
}

// Função Union com União por Rank (Union by Rank)
bool UnionFind::unite(int i, int j) {
    int raiz_i = find(i);
    int raiz_j = find(j);

    if (raiz_i != raiz_j) {
        // Une a árvore menor sob a raiz da árvore maior
        if (rank[raiz_i] < rank[raiz_j]) {
            parent[raiz_i] = raiz_j;
        } else if (rank[raiz_i] > rank[raiz_j]) {
            parent[raiz_j] = raiz_i;
        } else {
            parent[raiz_j] = raiz_i;
            rank[raiz_i]++;
        }
        return true; // Unificação feita com sucesso (sem ciclo)
    }
    return false; // Já pertencem ao mesmo conjunto (Ciclo detectado!)
}