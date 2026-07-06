#include "../../include/Graph.hpp"
#include <fstream>
#include <algorithm>

Graph lerInstancia(string arquivo, int& numVertices, int& numArestas, int& numRotulos) {
    ifstream in(arquivo);

    in >> numVertices >> numArestas >> numRotulos;

    Graph g(false);
    vector<Edge> arestas;

    for (int i = 0; i < numArestas; i++) {
        int u, v, rotulo;
        in >> u >> v >> rotulo;
        arestas.push_back({u, v, (double)rotulo});
    }

    contructGraphFromEdges(g, arestas);
    return g;
}

int main() {
    for (int i = 1; i <= 20; i++) {
        string numero = (i < 10 ? "0" : "") + to_string(i);
        string arquivo = "instancias/instancia" + numero + ".txt";

        int numVertices, numArestas, numRotulos;
        Graph g = lerInstancia(arquivo, numVertices, numArestas, numRotulos);

        string resultado = g.minimumLabelingSpanningTree();
        // pega só a primeira linha (ex: "Rotulos utilizados: 3")
        string resumo = resultado.substr(0, resultado.find('\n'));

        cout << "Instancia: " << arquivo
             << "  | vertices=" << numVertices
             << "  arestas=" << numArestas
             << "  rotulos=" << numRotulos
             << "  | " << resumo << "\n";
    }

    return 0;
}
