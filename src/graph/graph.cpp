#include "../../include/Graph.hpp"
#include "../disjoint-set-union/UnionFind.h"
#include <algorithm>
#include <functional>
#include <sstream>
 
// ============================== No ==============================
 
No::No(int id) : id(id), visitado(false), grau_entrada(0), grau_saida(0) {}
 
// ============================== Graph: infraestrutura ==============================
 
Graph::Graph(bool direcionado) : num_vertices(0), direcionado(direcionado) {}
 
Graph::~Graph() {
    for (No* no : nos) delete no;
}
 
void Graph::limpar_visitados() {
    for (No* no : nos) no->visitado = false;
}
 
No* Graph::buscar_no(int id) {
    for (No* no : nos) {
        if (no->id == id) return no;
    }
    return nullptr;
}
 
// ============================== Vértices ==============================
 
bool Graph::insertVertex(int vertex) {
    if (buscar_no(vertex) != nullptr) return false; // já existe
    nos.push_back(new No(vertex));
    num_vertices++;
    return true;
}
 
bool Graph::removeVertex(int vertex) {
    No* alvo = buscar_no(vertex);
    if (alvo == nullptr) return false;
 
    // Remove todas as referências a este vértice na vizinhança dos demais
    for (No* no : nos) {
        auto& viz = no->vizinhos;
        viz.erase(remove(viz.begin(), viz.end(), alvo), viz.end());
    }
 
    // Remove pesos associados
    for (auto it = pesos.begin(); it != pesos.end();) {
        if (it->first.first == vertex || it->first.second == vertex) {
            it = pesos.erase(it);
        } else {
            ++it;
        }
    }
 
    nos.erase(remove(nos.begin(), nos.end(), alvo), nos.end());
    delete alvo;
    num_vertices--;
    return true;
}
 
// ============================== Arestas ==============================
 
void Graph::adicionar_aresta(int u, int v, double weight, bool dir) {
    No* nu = buscar_no(u);
    No* nv = buscar_no(v);
    if (nu == nullptr) { insertVertex(u); nu = buscar_no(u); }
    if (nv == nullptr) { insertVertex(v); nv = buscar_no(v); }
 
    nu->vizinhos.push_back(nv);
    nu->grau_saida++;
    nv->grau_entrada++;
    pesos[{u, v}] = weight;
 
    if (!dir) {
        nv->vizinhos.push_back(nu);
        nv->grau_saida++;
        nu->grau_entrada++;
        pesos[{v, u}] = weight;
    }
}
 
bool Graph::insertEdge(int outputVertex, int inputVertex) {
    if (existEdge(outputVertex, inputVertex)) return false;
 
    No* u = buscar_no(outputVertex);
    No* v = buscar_no(inputVertex);
    if (u == nullptr) { insertVertex(outputVertex); u = buscar_no(outputVertex); }
    if (v == nullptr) { insertVertex(inputVertex); v = buscar_no(inputVertex); }
 
    u->vizinhos.push_back(v);
    u->grau_saida++;
    v->grau_entrada++;
    pesos[{outputVertex, inputVertex}] = 1.0;
 
    if (!direcionado) {
        v->vizinhos.push_back(u);
        v->grau_saida++;
        u->grau_entrada++;
        pesos[{inputVertex, outputVertex}] = 1.0;
    }
    return true;
}
 
bool Graph::removeEdge(int outputVertex, int inputVertex) {
    No* u = buscar_no(outputVertex);
    No* v = buscar_no(inputVertex);
    if (u == nullptr || v == nullptr) return false;
 
    auto& vizU = u->vizinhos;
    auto itU = find(vizU.begin(), vizU.end(), v);
    if (itU == vizU.end()) return false; // aresta não existe
 
    vizU.erase(itU);
    u->grau_saida--;
    v->grau_entrada--;
    pesos.erase({outputVertex, inputVertex});
 
    if (!direcionado) {
        auto& vizV = v->vizinhos;
        auto itV = find(vizV.begin(), vizV.end(), u);
        if (itV != vizV.end()) vizV.erase(itV);
        v->grau_saida--;
        u->grau_entrada--;
        pesos.erase({inputVertex, outputVertex});
    }
    return true;
}
 
bool Graph::existEdge(int outputVertex, int inputVertex) {
    No* u = buscar_no(outputVertex);
    if (u == nullptr) return false;
    for (No* viz : u->vizinhos) {
        if (viz->id == inputVertex) return true;
    }
    return false;
}
 
bool Graph::changeWeight(double weight, int outputVertex, int inputVertex) {
    if (!existEdge(outputVertex, inputVertex)) return false;
    pesos[{outputVertex, inputVertex}] = weight;
    if (!direcionado) pesos[{inputVertex, outputVertex}] = weight;
    return true;
}
 
bool Graph::verifyAdjacency(int vertex1, int vertex2) {
    return existEdge(vertex1, vertex2) || existEdge(vertex2, vertex1);
}
 
int Graph::calculateDegree(int vertex) {
    No* no = buscar_no(vertex);
    if (no == nullptr) return -1;
    if (direcionado) return no->grau_entrada + no->grau_saida;
    return static_cast<int>(no->vizinhos.size());
}
 
vector<int> Graph::listNeighbors(int vertex) {
    vector<int> resultado;
    No* no = buscar_no(vertex);
    if (no == nullptr) return resultado;
    for (No* viz : no->vizinhos) resultado.push_back(viz->id);
    return resultado;
}
 
// ============================== Exibição ==============================
 
void Graph::show() {
    for (No* no : nos) {
        cout << no->id << " -> ";
        for (No* viz : no->vizinhos) cout << viz->id << " ";
        cout << "\n";
    }
}
 
void Graph::imprimir() {
    for (No* no : nos) {
        cout << no->id << ": ";
        for (No* viz : no->vizinhos) {
            double peso = 1.0;
            auto it = pesos.find({no->id, viz->id});
            if (it != pesos.end()) peso = it->second;
            cout << viz->id << "(w=" << peso << ") ";
        }
        cout << "\n";
    }
}
 
void Graph::imprimir(std::ostream& out) {
    for (No* no : nos) {
        out << no->id << ": ";
        for (No* viz : no->vizinhos) {
            double peso = 1.0;
            auto it = pesos.find({no->id, viz->id});
            if (it != pesos.end()) peso = it->second;
            out << viz->id << "(w=" << peso << ") ";
        }
        out << "\n";
    }
}

// ============================== Kruskal (MST) ==============================
 
string Graph::kruskal() {
    if (direcionado) return "Kruskal nao se aplica a grafos direcionados.";
 
    struct Aresta { int u, v; double peso; };
    vector<Aresta> arestas;
    set<pair<int, int>> vistas;
 
    for (No* no : nos) {
        for (No* viz : no->vizinhos) {
            int a = min(no->id, viz->id);
            int b = max(no->id, viz->id);
            if (vistas.count({a, b})) continue;
            vistas.insert({a, b});
            double peso = 1.0;
            auto it = pesos.find({no->id, viz->id});
            if (it != pesos.end()) peso = it->second;
            arestas.push_back({a, b, peso});
        }
    }
    sort(arestas.begin(), arestas.end(),
         [](const Aresta& x, const Aresta& y) { return x.peso < y.peso; });
 
    map<int, int> pai;
    for (No* no : nos) pai[no->id] = no->id;
    function<int(int)> find_raiz = [&](int x) -> int {
        while (pai[x] != x) { pai[x] = pai[pai[x]]; x = pai[x]; }
        return x;
    };
 
    ostringstream saida;
    double custoTotal = 0.0;
    for (auto& a : arestas) {
        int ra = find_raiz(a.u), rb = find_raiz(a.v);
        if (ra != rb) {
            pai[ra] = rb;
            custoTotal += a.peso;
            saida << a.u << " -- " << a.v << " (peso " << a.peso << ")\n";
        }
    }
    saida << "Custo total da MST: " << custoTotal;
    return saida.str();
}
 
// ============================== MVCA ==============================
//
// Algoritmo 2-aproximativo para Cobertura Mínima de Vértices.
//   Enquanto houver arestas:
//     - escolhe-se uma aresta (u, v) qualquer;
//     - adicionam-se u e v à cobertura;
//     - removem-se todas as arestas incidentes a u ou a v.
 
vector<int> Graph::mvca() {
    // Cópia local da lista de adjacência (por id), para remover arestas
    // sem alterar a estrutura original do grafo.
    map<int, set<int>> adjRestante;
    for (No* no : nos) {
        set<int> vizinhosIds;
        for (No* viz : no->vizinhos) vizinhosIds.insert(viz->id);
        adjRestante[no->id] = vizinhosIds;
    }
 
    vector<int> cobertura;
    set<int> naCobertura;
 
    while (true) {
        // Procura um vértice com pelo menos uma aresta restante
        int u = -1;
        for (auto& par : adjRestante) {
            if (!par.second.empty()) {
                u = par.first;
                break;
            }
        }
        if (u == -1) break; // não há mais arestas: terminou
 
        int v = *adjRestante[u].begin(); // escolhe a aresta (u, v)
 
        if (naCobertura.insert(u).second) cobertura.push_back(u);
        if (naCobertura.insert(v).second) cobertura.push_back(v);
 
        // Remove todas as arestas incidentes a u ou a v
        for (int viz : adjRestante[u]) adjRestante[viz].erase(u);
        for (int viz : adjRestante[v]) adjRestante[viz].erase(v);
        adjRestante[u].clear();
        adjRestante[v].clear();
    }
 
    return cobertura;
}
 
bool Graph::validarCobertura(const vector<int>& cobertura) {
    set<int> conjunto(cobertura.begin(), cobertura.end());
 
    set<pair<int, int>> vistas;
    for (No* no : nos) {
        for (No* viz : no->vizinhos) {
            int a = min(no->id, viz->id);
            int b = max(no->id, viz->id);
            if (vistas.count({a, b})) continue;
            vistas.insert({a, b});
            if (!conjunto.count(a) && !conjunto.count(b)) return false;
        }
    }
    return true;
}

void contructGraphFromEdges(Graph& graph, const std::vector<Edge>& edges) {
    for (const auto& edge : edges) {
        graph.adicionar_aresta(edge.u, edge.v, edge.label, false);
    }
}

void listofEdges(const Graph& graph, std::vector<Edge>& edges) {
    map<pair<int,int>, double> pesos = graph.getPesos();
    for (const auto& edge : pesos) {
        edges.push_back({edge.first.first, edge.first.second, edge.second});
    }
}

void Graph::cutCycles(const std::vector<Edge>& arestas) {
    UnionFind uf(num_vertices);
    map<pair<int, int>, double> arvore_final;
    std::vector<Edge> edgesToRemove;

    for (const auto& edge : arestas) {
        if (!uf.unite(edge.u, edge.v)) {
            edgesToRemove.push_back(edge);
        } else {
            arvore_final[{edge.u, edge.v}] = edge.label;
        }
    }

    // Aplica a remoção de fato no grafo
    for (const auto& edge : edgesToRemove) {
        removeEdge(edge.u, edge.v);
    }

}

std::string Graph::minimumLabelingSpanningTree() {
    std::vector<Edge> totalEdges;
    listofEdges(*this, totalEdges);

    std::vector<Edge> edges;
    for (const auto& e : totalEdges) {
        if (e.u < e.v) edges.push_back(e);
    }

    // Agrupa arestas por rótulo
    map<double, std::vector<Edge>> forLabel;
    for (const auto& e : edges) forLabel[e.label].push_back(e);

    Graph subgrafo(false);
    for (No* no : nos) subgrafo.insertVertex(no->id); // garante todos os vértices presentes

    set<int> verticesCobertos;
    set<double> rotulosUsados;
    std::vector<double> ordemRotulos;
    size_t totalVertices = nos.size();
 
    while (verticesCobertos.size() < totalVertices) {
        double melhorRotulo = 0.0;
        bool achouRotulo = false;
        int melhorGanho = -1;
 
        for (const auto& par : forLabel) {
            double rotulo = par.first;
            if (rotulosUsados.count(rotulo)) continue;
 
            set<int> ganho;
            for (const auto& e : par.second) {
                if (!verticesCobertos.count(e.u)) ganho.insert(e.u);
                if (!verticesCobertos.count(e.v)) ganho.insert(e.v);
            }
            if (static_cast<int>(ganho.size()) > melhorGanho) {
                melhorGanho = static_cast<int>(ganho.size());
                melhorRotulo = rotulo;
                achouRotulo = true;
            }
        }
 
        if (!achouRotulo || melhorGanho <= 0) break; // nenhum rótulo restante amplia a cobertura
 
        rotulosUsados.insert(melhorRotulo);
        ordemRotulos.push_back(melhorRotulo);
        for (const auto& e : forLabel[melhorRotulo]) {
            subgrafo.adicionar_aresta(e.u, e.v, e.label, false);
            verticesCobertos.insert(e.u);
            verticesCobertos.insert(e.v);
        }
    }
 
    // Checagem de completude reaproveitando mvca/validarCobertura como sinal
    // estrutural de que o subgrafo cobre todos os vértices.
    vector<int> cobertura = subgrafo.mvca();
    bool completo = subgrafo.validarCobertura(cobertura)
                    && verticesCobertos.size() == totalVertices;
 
    std::ostringstream oss;
    if (!completo) {
        oss << "Nao foi possivel cobrir todos os vertices com os rotulos disponiveis.\n";
        return oss.str();
    }
 
    // Remove ciclos remanescentes do subgrafo escolhido por rótulos mínimos
    std::vector<Edge> arestasSubgrafo;
    for (double rotulo : ordemRotulos) {
        for (const auto& e : forLabel[rotulo]) {
            arestasSubgrafo.push_back(e);
        }
    }

    subgrafo.cutCycles(arestasSubgrafo);
 
    oss << "Rotulos utilizados: " << rotulosUsados.size() << "\n";
    subgrafo.imprimir(oss);
    return oss.str();
}