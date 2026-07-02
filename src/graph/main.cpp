#include "Graph.hpp"
#include <algorithm>

int main() {
    Graph g(false); // grafo não direcionado

    vector<pair<int, int>> arestas = {
        {0, 1}, {0, 2}, {1, 2},
        {2, 3}, {3, 4}, {4, 5},
        {5, 3}, {6, 5}
    };
    for (size_t i = 0; i < arestas.size(); ++i) {
        g.insertEdge(arestas[i].first, arestas[i].second);
    }

    cout << "Grafo (lista de adjacencia):\n";
    g.imprimir();

    vector<int> cobertura = g.mvca();
    sort(cobertura.begin(), cobertura.end());

    cout << "\nCobertura de vertices (MVCA): ";
    for (int v : cobertura) cout << v << " ";
    cout << "\n";

    cout << "Tamanho da cobertura: " << cobertura.size() << "\n";
    cout << "Cobertura valida? " << (g.validarCobertura(cobertura) ? "sim" : "nao") << "\n";

    return 0;
}