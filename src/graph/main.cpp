#include "Graph.hpp"
#include <fstream>
#include <algorithm>

Graph lerInstancia(string arquivo, int& numVertices, int& numArestas, int& numRotulos) {
    ifstream in(arquivo);

    in >> numVertices >> numArestas >> numRotulos;

    Graph g(false);

    for (int i = 0; i < numArestas; i++) {
        int u, v, rotulo;
        in >> u >> v >> rotulo;
        g.insertEdge(u, v);
        g.changeWeight(rotulo, u, v);
    }

    return g;
}

int main() {
    for (int i = 1; i <= 20; i++) {
        string numero = (i < 10 ? "0" : "") + to_string(i);
        string arquivo = "instancias/instancia" + numero + ".txt";

        int numVertices, numArestas, numRotulos;
        Graph g = lerInstancia(arquivo, numVertices, numArestas, numRotulos);

        cout << "Instancia: " << arquivo
             << "  | vertices=" << numVertices
             << "  arestas=" << numArestas
             << "  rotulos=" << numRotulos << "\n";
    }

    return 0;
}
