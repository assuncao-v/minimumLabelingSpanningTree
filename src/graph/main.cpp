#include "../../include/Graph.hpp"
#include <algorithm>
 
int main() {
    Graph g(false); // grafo não direcionado
 
    std::vector<Edge> arestas = {
        {0, 1, 1.0}, {0, 2, 1.0}, {1, 2, 1.0},
        {2, 3, 1.0}, {3, 4, 1.0}, {4, 5, 1.0},
        {5, 3, 2.0}, {6, 5, 2.0}, {1, 3, 2.0}, {2, 4, 2.0}, {0, 5, 3.0}
    };
 
    contructGraphFromEdges(g, arestas);
    std::cout << "Grafo (lista de adjacencia):\n";
    g.imprimir();
    
    std::cout << "Subgrafo de rótulos mínimos:\n";
    std::cout << g.minimumLabelingSpanningTree() << "\n";
 
 
    return 0;
}
