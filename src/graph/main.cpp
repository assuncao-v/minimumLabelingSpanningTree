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
        arestas.push_back({u, v, (int)rotulo});
    }

    contructGraphFromEdges(g, arestas);
    return g;
}

int main() {
    srand(time(NULL));
    double alpha[5] = {0.05, 0.1, 0.15, 0.3, 0.5};
    for (int i = 1; i <= 20; i++) {
        string numero = (i < 10 ? "0" : "") + to_string(i);
        string arquivo = "instancias/instancia" + numero + ".txt";

        int numVertices, numArestas, numRotulos;
        Graph g = lerInstancia(arquivo, numVertices, numArestas, numRotulos);

        string resultado = g.minimumLabelingSpanningTree();
        vector<string> linhasResultado;
        size_t inicio = 0;
        while (inicio <= resultado.size()) {
            size_t fim = resultado.find('\n', inicio);
            if (fim == string::npos) {
                if (inicio < resultado.size()) {
                    linhasResultado.push_back(resultado.substr(inicio));
                }
                break;
            }

            linhasResultado.push_back(resultado.substr(inicio, fim - inicio));
            inicio = fim + 1;
        }

        cout << "Instancia: " << arquivo
            << "  | vertices=" << numVertices
            << "  arestas=" << numArestas
            << "  rotulos=" << numRotulos
            << "  | ";

        if (!linhasResultado.empty()) {
            cout << linhasResultado[0] << "\n";
            for (size_t i = 1; i < 2; i++) {
                cout << "  | " << linhasResultado[i] << "\n";
            }
        } else {
            cout << "\n";
        }

        for (double a : alpha) {
            string resultadoRandomizado = g.minimumLabelingSpanningTreeRandomized(a);
            vector<string> linhasResultadoRandom;
            size_t inicio = 0;
            while (inicio <= resultadoRandomizado.size()) {
                size_t fim = resultadoRandomizado.find('\n', inicio);
                if (fim == string::npos) {
                    if (inicio < resultadoRandomizado.size()) {
                        linhasResultadoRandom.push_back(resultadoRandomizado.substr(inicio));
                    }
                    break;
                }

                linhasResultadoRandom.push_back(resultadoRandomizado.substr(inicio, fim - inicio));
                inicio = fim + 1;
            }
            if (!linhasResultadoRandom.empty()) {
                cout << linhasResultadoRandom[0] << "\n";
                for (size_t i = 1; i < 2; i++) {
                    cout << "  | " << linhasResultadoRandom[i] << "\n";
                }
            } else {
                cout << "\n";
            }
        }
    }
    
    return 0;
}