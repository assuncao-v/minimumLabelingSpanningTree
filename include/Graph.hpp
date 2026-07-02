#ifndef GRAPH_HPP
#define GRAPH_HPP
 
using namespace std;
 
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <string>
#include <map>
#include <set>
#include <utility>
 
class No{
    public:
        int id;
        bool visitado;
        int grau_entrada;
        int grau_saida;
        vector<No*> vizinhos;
        No(int id);
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
        void adicionar_aresta(int u, int v, bool dir = false);
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
 
        vector<int> mvca();
 
        bool validarCobertura(const vector<int>& cobertura);
};
 
#endif