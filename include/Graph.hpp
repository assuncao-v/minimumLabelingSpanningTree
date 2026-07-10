
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
    int label;

    // Ensina o C++ a comparar duas arestas
    bool operator==(const Edge& other) const {
        return (u == other.u && v == other.v && label == other.label);
    }
};
 
class Graph{
    private:
        int num_vertices;
        bool direcionado;
        vector<No*> nos;
        map<pair<int,int>, std::vector<int>> pesos;
        vector<Edge> arestas;
 
        void limpar_visitados();
        No* buscar_no(int id);
    public:
        Graph(bool direcionado = false);
        ~Graph();
        map<pair<int,int>, std::vector<int>> getPesos() const { return pesos; }
        const vector<No*>& getNos() const { return nos; }
        int getNumVertices() const { return num_vertices; }
        const vector<Edge>& getArestas() const { return arestas; }
        void adicionar_aresta(int u, int v, int weight, bool dir = false);
        bool insertVertex(int vertex);
        bool removeVertex(int vertex);
        bool insertEdge(int outputVertex, int inputVertex, int weight);
        bool removeEdge(int outputVertex, int inputVertex, int weight);
        bool existEdge(int outputVertex, int inputVertex);
        bool changeWeight(int weight, int outputVertex, int inputVertex);
        bool verifyAdjacency(int vertex1, int vertex2);
        bool verifyDirected() { return direcionado; }
        int  calculateDegree(int vertex);
        vector<int> listNeighbors(int vertex);
        void show();
        string kruskal();
        void imprimir();
        void imprimir(std::ostream& os);
 
        vector<int> mvca(int maxVertexId, double alpha = 0.0);
        bool validarCobertura(const vector<int>& cobertura);
        bool validarCoberturaRotulos(const std::vector<Edge>& cobertura, int maxVertexId);
        void cutCycles(std::vector<Edge>& arestas, int maxVertexId);
 
        string minimumLabelingSpanningTree();
        string minimumLabelingSpanningTreeRandomized(double alpha = 1.0);

        string minimumLabelingSpanningTreeGRASP();
};
 
void contructGraphFromEdges(Graph& graph, const std::vector<Edge>& edges);
void listofEdges(const Graph& graph, std::vector<Edge>& edges);

#endif // GRAPH_HPP