#include "Graph.hpp"

#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <sstream>

using namespace std;

// ============================================================================
// No
// ============================================================================

No::No(int id) {
    this->id = id;
    this->visitado = false;
    this->grau_entrada = 0;
    this->grau_saida = 0;
}

// ============================================================================
// Graph — privados
// ============================================================================

No* Graph::buscar_no(int id) {
    for (No* no : nos) 
        if (no->id == id) return no;
    return nullptr;
}

void Graph::limpar_visitados() {
    for (No* no : nos) 
        no->visitado = false;
}

// ============================================================================
// Graph — construtor / destrutor
// ============================================================================

Graph::Graph(bool direcionado) {
    this->direcionado = direcionado;
    this->num_vertices = 0;
}

Graph::~Graph() {
    for (No* no : nos)
        delete no;
}

// ============================================================================
// adicionar_aresta (legado — acesso direto por índice)
// ============================================================================

void Graph::adicionar_aresta(int u, int v, bool dir) {
    if (dir) {
        nos[u]->vizinhos.push_back(nos[v]);
        nos[u]->grau_saida++;
        nos[v]->grau_entrada++;
    } else {
        nos[u]->vizinhos.push_back(nos[v]);
        nos[v]->vizinhos.push_back(nos[u]);
        nos[u]->grau_entrada++;
        nos[v]->grau_entrada++;
    }
}

// ============================================================================
// insertVertex
// ============================================================================

bool Graph::insertVertex(int vertex) {
    if (buscar_no(vertex) != nullptr) return false;

    nos.push_back(new No(vertex));
    num_vertices++;
    return true;
}

// ============================================================================
// removeVertex
// ============================================================================

bool Graph::removeVertex(int vertex) {
    // Encontra o índice do vértice no vetor nos
    int indice = -1;
    for (int i = 0; i < (int)nos.size(); i++) {
        if (nos[i]->id == vertex) { indice = i; break; }
    }
    if (indice == -1) return false;

    No* noRemover = nos[indice];

    // Passo 1: remove vertex da lista de vizinhos de todos os outros nós e ajusta graus
    for (No* no : nos) {
        if (no->id == vertex) continue;
        for (int i = 0; i < (int)no->vizinhos.size(); i++) {
            if (no->vizinhos[i]->id == vertex) {
                no->vizinhos.erase(no->vizinhos.begin() + i);
                // Dirigido: quem aponta perde grau_saida; não-direcionado: perde grau_entrada
                direcionado ? no->grau_saida-- : no->grau_entrada--;
                i--;
            }
        }
    }

    // Passo 2: apenas para grafos direcionados — decrementa grau_entrada dos nós que vertex apontava
    // (para não-direcionados o passo 1 já tratou ambos os lados)
    if (direcionado) {
        for (No* vizinho : noRemover->vizinhos) {
            vizinho->grau_entrada--;
        }
    }

    // Remove pesos de arestas envolvendo vertex
    for (auto it = pesos.begin(); it != pesos.end(); ) {
        if (it->first.first == vertex || it->first.second == vertex)
            it = pesos.erase(it);
        else
            ++it;
    }

    // Passo 3: libera memória e remove do vetor
    delete noRemover;
    nos.erase(nos.begin() + indice);
    num_vertices--;

    return true;
}

// ============================================================================
// insertEdge
// ============================================================================

bool Graph::insertEdge(int outputVertex, int inputVertex) {
    No* noSaida  = buscar_no(outputVertex);
    No* noEntrada = buscar_no(inputVertex);

    if (noSaida == nullptr || noEntrada == nullptr) return false;

    // Verifica duplicata
    for (No* v : noSaida->vizinhos) {
        if (v->id == inputVertex) return false;
    }

    if (direcionado) {
        // Aresta dirigida: outputVertex → inputVertex
        noSaida->vizinhos.push_back(noEntrada);
        noSaida->grau_saida++;
        noEntrada->grau_entrada++;
    } else {
        // Aresta não dirigida: adiciona nos dois lados, incrementa grau_entrada de ambos
        noSaida->vizinhos.push_back(noEntrada);
        noEntrada->vizinhos.push_back(noSaida);
        noSaida->grau_entrada++;
        noEntrada->grau_entrada++;
    }

    return true;
}

// ============================================================================
// removeEdge
// ============================================================================

bool Graph::removeEdge(int outputVertex, int inputVertex) {
    No* noSaida  = buscar_no(outputVertex);
    No* noEntrada = buscar_no(inputVertex);

    if (noSaida == nullptr || noEntrada == nullptr) return false;

    // Remove inputVertex dos vizinhos de outputVertex
    bool encontrou = false;
    for (int i = 0; i < (int)noSaida->vizinhos.size(); i++) {
        if (noSaida->vizinhos[i]->id == inputVertex) {
            noSaida->vizinhos.erase(noSaida->vizinhos.begin() + i);
            encontrou = true;
            break;
        }
    }
    if (!encontrou) return false;

    if (direcionado) {
        noSaida->grau_saida--;
        noEntrada->grau_entrada--;
        pesos.erase({outputVertex, inputVertex});
    } else {
        // Remove também o lado inverso
        for (int i = 0; i < (int)noEntrada->vizinhos.size(); i++) {
            if (noEntrada->vizinhos[i]->id == outputVertex) {
                noEntrada->vizinhos.erase(noEntrada->vizinhos.begin() + i);
                break;
            }
        }
        noSaida->grau_entrada--;
        noEntrada->grau_entrada--;
        pesos.erase({outputVertex, inputVertex});
        pesos.erase({inputVertex, outputVertex});
    }

    return true;
}

// ============================================================================
// existEdge
// ============================================================================

bool Graph::existEdge(int outputVertex, int inputVertex) {
    No* noSaida = buscar_no(outputVertex);
    if (noSaida == nullptr) return false;

    for (No* vizinho : noSaida->vizinhos) {
        if (vizinho->id == inputVertex) return true;
    }
    return false;
}

// ============================================================================
// changeWeight
// ============================================================================

bool Graph::changeWeight(double weight, int outputVertex, int inputVertex) {
    if (!existEdge(outputVertex, inputVertex)) return false;

    pesos[{outputVertex, inputVertex}] = weight;
    return true;
}

// ============================================================================
// verifyAdjacency
// ============================================================================

bool Graph::verifyAdjacency(int vertex1, int vertex2) {
    // Para não-direcionado ambas as direções estão em vizinhos, então existEdge(v1,v2) basta.
    // Para direcionado, verifica apenas a direção solicitada.
    return existEdge(vertex1, vertex2);
}

// ============================================================================
// calculateDegree
// ============================================================================

int Graph::calculateDegree(int vertex) {
    No* no = buscar_no(vertex);
    if (no == nullptr) return 0; // retorna 0 para vértice inexistente

    return no->grau_entrada + no->grau_saida;
}

// ============================================================================
// listNeighbors
// ============================================================================

vector<int> Graph::listNeighbors(int vertex) {
    vector<int> resultado;
    No* no = buscar_no(vertex);
    if (no == nullptr) return resultado;

    for (No* vizinho : no->vizinhos) {
        resultado.push_back(vizinho->id);
    }
    return resultado;
}

// ============================================================================
// show
// ============================================================================

void Graph::show() {
    cout << "Grafo (" << num_vertices << " vertices, "
         << (direcionado ? "direcionado" : "nao-direcionado") << "):" << endl;
    for (No* no : nos) {
        cout << "  [" << no->id << "]"
             << " grau_entrada=" << no->grau_entrada
             << " grau_saida="   << no->grau_saida
             << " -> ";
        for (No* vizinho : no->vizinhos) {
            cout << vizinho->id;
            auto it = pesos.find({no->id, vizinho->id});
            if (it != pesos.end()) cout << "(w=" << it->second << ")";
            cout << " ";
        }
        cout << endl;
    }
}

// ============================================================================
// Kruskal
// ============================================================================

// Estrutura auxiliar para o método
struct Edge {
    int u, v;
    double weight;
    
    // Operador para comparação de pesos entre arcos distintos.
    bool operator<(const Edge& secoundEdge) const {
        return this->weight < secoundEdge.weight;
    }
};

// ============================================================================
// imprimir (legado)
// ============================================================================

void Graph::imprimir() {
    for (No* no : nos) {
        cout << no->id << ": ";
        for (No* vizinho : no->vizinhos) {
            cout << vizinho->id << " ";
        }
        cout << endl;
    }
}