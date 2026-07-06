
#ifndef GRAPH_HPP
#define GRAPH_HPP
 
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <set>
#include <utility>
#include <map>
using namespace std;
 
class No{
    public:
        int id;
        bool visitado;
        int grau_entrada;
        int grau_saida;
        vector<No*> vizinhos;
        No(int id);
};
 
struct Edge {
    int u, v;
    double label;
};
 
class Graph{
    private:
        int num_vertices;
        bool direcionado;
        vector<No*> nos;
        map<pair<int,int>, double> pesos;
 
        void limpar_visitados();
        No* buscar_no(int id);
    public:
        Graph(bool direcionado = false);
        ~Graph();
        map<pair<int,int>, double> getPesos() const { return pesos; }
        const vector<No*>& getNos() const { return nos; }
        int getNumVertices() const { return num_vertices; }
        void adicionar_aresta(int u, int v, double weight, bool dir = false);
        bool insertVertex(int vertex);
        bool removeVertex(int vertex);
        bool insertEdge(int outputVertex, int inputVertex);
        bool removeEdge(int outputVertex, int inputVertex);
        bool existEdge(int outputVertex, int inputVertex);
        bool changeWeight(double weight, int outputVertex, int inputVertex);
        bool verifyAdjacency(int vertex1, int vertex2);
        bool verifyDirected() { return direcionado; }
        int  calculateDegree(int vertex);
        vector<int> listNeighbors(int vertex);
        void show();
        string kruskal();
        void imprimir();
        void imprimir(std::ostream& os);
 
        vector<int> mvca();
        bool validarCobertura(const vector<int>& cobertura);
        void cutCycles(const std::vector<Edge>& arestas);
 
        string minimumLabelingSpanningTree();
};
 
void contructGraphFromEdges(Graph& graph, const std::vector<Edge>& edges);
void listofEdges(const Graph& graph, std::vector<Edge>& edges);

#endif // GRAPH_HPP