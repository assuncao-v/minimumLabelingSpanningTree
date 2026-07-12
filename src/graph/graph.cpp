#include "../../include/Graph.hpp"
#include "../disjoint-set-union/UnionFind.h"
#include <algorithm>
#include <functional>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <cstdlib> // Para rand()
#include <ctime>   // Para time()
#include <sstream>
#include <limits>
#include <algorithm>
#include <random>
#include <numeric>
 
// ============================== Estruturas auxiliares ==============================
struct Candidate {
    int rotulo;
    int reducao;
};

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
 
void Graph::adicionar_aresta(int u, int v, int weight, bool dir) {
    No* nu = buscar_no(u);
    No* nv = buscar_no(v);
    if (nu == nullptr) { insertVertex(u); nu = buscar_no(u); }
    if (nv == nullptr) { insertVertex(v); nv = buscar_no(v); }
 
    nu->vizinhos.push_back(nv);
    nu->grau_saida++;
    nv->grau_entrada++;
    pesos[{u, v}].push_back(weight);
    arestas.push_back({u, v, weight});

    if (!dir) {
        nv->vizinhos.push_back(nu);
        nv->grau_saida++;
        nu->grau_entrada++;
        pesos[{v, u}].push_back(weight);
        arestas.push_back({v, u, weight});
    }
}
 
bool Graph::insertEdge(int outputVertex, int inputVertex, int weight) {
    if (existEdge(outputVertex, inputVertex)) return false;
 
    No* u = buscar_no(outputVertex);
    No* v = buscar_no(inputVertex);
    if (u == nullptr) { insertVertex(outputVertex); u = buscar_no(outputVertex); }
    if (v == nullptr) { insertVertex(inputVertex); v = buscar_no(inputVertex); }
 
    u->vizinhos.push_back(v);
    u->grau_saida++;
    v->grau_entrada++;
    pesos[{outputVertex, inputVertex}].push_back(weight);
 
    if (!direcionado) {
        v->vizinhos.push_back(u);
        v->grau_saida++;
        u->grau_entrada++;
        pesos[{inputVertex, outputVertex}].push_back(weight);
    }
    return true;
}
 
bool Graph::removeEdge(int outputVertex, int inputVertex, int weight) {
    No* u = buscar_no(outputVertex);
    No* v = buscar_no(inputVertex);
    if (u == nullptr || v == nullptr) return false;
 
    auto& vizU = u->vizinhos;
    auto itU = find(vizU.begin(), vizU.end(), v);
    if (itU == vizU.end()) return false; // aresta não existe
 
    vizU.erase(itU);
    u->grau_saida--;
    v->grau_entrada--;
    auto it = pesos.find({outputVertex, inputVertex});
    if (it != pesos.end()) {
        auto& weights = it->second;
        auto weight_it = find(weights.begin(), weights.end(), weight);
        if (weight_it != weights.end()) {
            weights.erase(weight_it);
        }
    }
 
    if (!direcionado) {
        auto& vizV = v->vizinhos;
        auto itV = find(vizV.begin(), vizV.end(), u);
        if (itV != vizV.end()) vizV.erase(itV);
        v->grau_saida--;
        u->grau_entrada--;
        auto it_rev = pesos.find({inputVertex, outputVertex});
        if (it_rev != pesos.end()) {
            auto& weights_rev = it_rev->second;
            auto weight_it_rev = find(weights_rev.begin(), weights_rev.end(), weight);
            if (weight_it_rev != weights_rev.end()) {
                weights_rev.erase(weight_it_rev);
            }
        }
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
 
bool Graph::changeWeight(int weight, int outputVertex, int inputVertex) {
    if (!existEdge(outputVertex, inputVertex)) return false;
    pesos[{outputVertex, inputVertex}].push_back(weight);
    if (!direcionado) pesos[{inputVertex, outputVertex}].push_back(weight);
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
            int peso = 0;
            auto it = pesos.find({no->id, viz->id});
            if (it != pesos.end()) peso = it->second[0];
            cout << viz->id << "(w=" << peso << ") ";
        }
        cout << "\n";
    }
}
 
void Graph::imprimir(std::ostream& out) {
    for (No* no : nos) {
        out << no->id << ": ";
        for (No* viz : no->vizinhos) {
            int peso = 0;
            auto it = pesos.find({no->id, viz->id});
            if (it != pesos.end()) peso = it->second[0];
            out << viz->id << "(w=" << peso << ") ";
        }
        out << "\n";
    }
}

// ============================== Kruskal (MST) ==============================
 
string Graph::kruskal() {
    if (direcionado) return "Kruskal nao se aplica a grafos direcionados.";
 
    struct Aresta { int u, v; int peso; };
    vector<Aresta> arestas;
    set<pair<int, int>> vistas;
 
    for (No* no : nos) {
        for (No* viz : no->vizinhos) {
            int a = min(no->id, viz->id);
            int b = max(no->id, viz->id);
            if (vistas.count({a, b})) continue;
            vistas.insert({a, b});
            int peso = 1.0;
            auto it = pesos.find({no->id, viz->id});
            if (it != pesos.end()) peso = it->second[0];
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
// Algoritmo 2-aproximativo para Cobertura Máxima de Vértices.
//   Enquanto houver arestas:
//     - escolhe-se uma aresta (u, v) qualquer;
//     - adicionam-se u e v à cobertura;
//     - removem-se todas as arestas incidentes a u ou a v.
 
std::vector<int> Graph::mvca(int maxVertexId, double alpha) {
    std::vector<Edge> totalEdges;
    listofEdges(*this, totalEdges);

    // Agrupa arestas por rótulo (evitando duplicatas verificando u < v)
    map<int, std::vector<Edge>> porRotulo;
    for (const auto& e : totalEdges) {
        if (e.u < e.v) porRotulo[e.label].push_back(e);
    }

    set<int> rotulosDisponiveis;
    for (const auto& par : porRotulo) {
        rotulosDisponiveis.insert(par.first);
    }

    UnionFind uf(maxVertexId + 1); // +1 caso os vértices sejam 1-indexados
    int componentes = num_vertices; // Inicialmente, cada vértice é sua própria componente
    vector<int> rotulosEscolhidos;

    while (componentes > 1 && !rotulosDisponiveis.empty()) {
        vector<Candidate> candidatos;

        for(int rotulo : rotulosDisponiveis) {
            UnionFind temp_uf = uf; // Cópia do estado atual para simulação
            int reducao = 0;

            for (const auto& e : porRotulo[rotulo]) {
                if (temp_uf.unite(e.u, e.v)) {
                    reducao++;
                }
            }

            if (reducao > 0) {
                candidatos.push_back({rotulo, reducao});
            }
        }

        if (candidatos.empty()) break;

        sort(candidatos.begin(), candidatos.end(), [](const Candidate& a, const Candidate& b) {
            return a.reducao > b.reducao;
        });

        int limiteMax = std::max(0, (int)(alpha * candidatos.size()) - 1);
        int k = (limiteMax == 0) ? 0 : rand() % (limiteMax + 1);
        int melhorRotulo = candidatos[k].rotulo;

        rotulosEscolhidos.push_back(melhorRotulo);
        rotulosDisponiveis.erase(melhorRotulo);
        
        for (const auto& e : porRotulo[melhorRotulo]) {
            if (uf.unite(e.u, e.v)) componentes--;
        }
    }

    return rotulosEscolhidos;
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

bool Graph::validarCoberturaRotulos(const std::vector<Edge>& cobertura, int maxVertexId) {
    // Inicializa o UnionFind com o número de vértices
    UnionFind uf(maxVertexId + 1);
    int componentes = num_vertices; // Inicialmente, cada vértice é sua própria componente

    // Tenta unir os vértices de cada aresta pertencente à árvore geradora
    for (const auto& e : cobertura) {
        if (uf.unite(e.u, e.v)) {
            componentes--; // Cada união bem-sucedida reduz uma componente
        }
    }

    // Se restou exatamente 1 componente, todos os vértices estão conectados!
    return (componentes == 1);
}

void contructGraphFromEdges(Graph& graph, const std::vector<Edge>& edges) {
    for (const auto& edge : edges) {
        graph.adicionar_aresta(edge.u, edge.v, edge.label, false);
    }
}

void listofEdges(const Graph& graph, std::vector<Edge>& edges) {
    edges = graph.getArestas();
}

void Graph::cutCycles(std::vector<Edge>& arestas, int maxVertexId) {
    UnionFind uf(maxVertexId + 1); // +1 porque vertices sao 1-indexados
    std::vector<Edge> edgesToRemove;

    for (auto& edge : arestas) {
        if (!uf.unite(edge.u, edge.v)) {
            edgesToRemove.push_back(edge);
        }
    }

    // Aplica a remoção de fato na lista
    for (auto& edge : edgesToRemove) {
        arestas.erase(std::remove(arestas.begin(), arestas.end(), edge), arestas.end());
    }
}

std::string Graph::minimumLabelingSpanningTree() {
    int maxVertexId = 0;
    for (No* no : nos) {
        if (no->id > maxVertexId) maxVertexId = no->id;
    }

    std::vector<int> rotulosEscolhidos = mvca(maxVertexId); // Obtém os rótulos escolhidos pelo algoritmo guloso
    // Criamos um set de inteiros para busca rápida O(log N) dos rótulos selecionados
    std::set<int> setRotulos(rotulosEscolhidos.begin(), rotulosEscolhidos.end());

    std::vector<Edge> totalEdges;
    listofEdges(*this, totalEdges); // Obtém todas as arestas do grafo original

    // 1. Filtramos as arestas VIRTUALMENTE (sem remover do grafo)
    std::vector<Edge> edgesFiltradas;
    for (const auto& e : totalEdges) {
        if (setRotulos.count(static_cast<int>(e.label))) {
            edgesFiltradas.push_back(e);
        }
    }

    // 2. Cut Cycles: removemos arestas que formam ciclos
    cutCycles(edgesFiltradas, maxVertexId);

    std::ostringstream oss;
    // 3. Validação segura: se não restou apenas 1 componente, o grafo não foi coberto
    if (validarCoberturaRotulos(edgesFiltradas, maxVertexId) == false) {
        oss << "Nao foi possivel cobrir todos os vertices com os rotulos disponiveis.\n";
        return oss.str();
    }
   
    // 4. Monta a string de retorno formatada
    oss << "Rotulos utilizados: " << rotulosEscolhidos.size() << "\n";
    oss << "Lista de rotulos escolhidos de forma gulosa: ";
    for (int r : rotulosEscolhidos) {
        oss << r << " ";
    }
    oss << "\nArestas componentes da arvore geradora:\n";
    for (const auto& e : edgesFiltradas) {
        oss << "  (" << e.u << " -> " << e.v << ") [Label: " << e.label << "]\n";
    }

    return oss.str();
}

/* ============================ Algoritmo guloso randomizado ============================

*/

std::string Graph::minimumLabelingSpanningTreeRandomized(double alpha) {
    int maxVertexId = 0;
    for (No* no : nos) if (no->id > maxVertexId) maxVertexId = no->id;

    int minRotulosGlobais = std::numeric_limits<int>::max();
    std::vector<int> melhorSolucaoGlobal;
    std::vector<Edge> melhorArvoreGlobal;
    bool encontrouSolucaoValida = false;

    int maxIter = 100;
    for (int iter = 0; iter < maxIter; iter++) {
        std::vector<int> rotulosEscolhidos = mvca(maxVertexId, alpha);
        std::set<int> setRotulos(rotulosEscolhidos.begin(), rotulosEscolhidos.end());

        std::vector<Edge> totalEdges;
        listofEdges(*this, totalEdges);

        std::vector<Edge> edgesFiltradas;
        for (const auto& e : totalEdges) {
            if (setRotulos.count(e.label)) edgesFiltradas.push_back(e);
        }
        cutCycles(edgesFiltradas, maxVertexId);

        if (validarCoberturaRotulos(edgesFiltradas, maxVertexId)) {
            encontrouSolucaoValida = true;
            if ((int)rotulosEscolhidos.size() < minRotulosGlobais) {
                minRotulosGlobais = rotulosEscolhidos.size();
                melhorSolucaoGlobal = rotulosEscolhidos;
                melhorArvoreGlobal = edgesFiltradas;
            }
        }
    }

    std::ostringstream oss;
    if (!encontrouSolucaoValida) {
        oss << "Nao foi possivel cobrir todos os vertices com os rotulos disponiveis.\n";
        return oss.str();
    }

    oss << "Rotulos utilizados: " << minRotulosGlobais << "\n";
    oss << "Lista de rotulos escolhidos de forma gulosa randomizada (alpha=" << alpha << "): ";
    for (int r : melhorSolucaoGlobal) oss << r << " ";
    oss << "\nArestas componentes da arvore geradora:\n";
    for (const auto& e : melhorArvoreGlobal) {
        oss << "  (" << e.u << " -> " << e.v << ") [Label: " << e.label << "]\n";
    }

    return oss.str();
}

/* ============================ Algoritmo guloso randomizado reativo ============================

*/
std::string Graph::minimumLabelingSpanningTreeGRASP(){
    int maxIter = 200;
    int blockSize = 20;

    int maxVertexId = 0;
    for (No* no: nos) if (no->id > maxVertexId) maxVertexId = no->id;

    std::vector<double> alphas = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    int numAlphas = alphas.size();
    std::vector<double> prob(numAlphas, 1.0 / numAlphas);

    // Informações para o cálculo das probabilidades de cada alfa
    std::vector<double> somaRotulos(numAlphas, 0.0);
    std::vector<int> contagens(numAlphas, 0);

    int minRotulosGlobais = std::numeric_limits<int>::max();
    std::vector<int> melhorSolucaoGlobal;
    std::vector<Edge> melhorArvoreGlobal;
    bool encontrouSolucaoValida = false;

    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < maxIter; i++) {
        // Heurística reativa
        std::discrete_distribution<> dist(prob.begin(), prob.end());
        int idxAlpha = dist(gen);
        double alphaAtual = alphas[idxAlpha];

        // Heurística gulosa randomizada
        std::vector<int> rotulosEscolhidos = mvca(maxVertexId, alphaAtual);
        std::set<int> setRotulos(rotulosEscolhidos.begin(), rotulosEscolhidos.end());

        std::vector<Edge> totalEdges;
        listofEdges(*this, totalEdges);

        std::vector<Edge> edgesFiltradas;
        for (const auto& e : totalEdges) {
            if (setRotulos.count(e.label)) edgesFiltradas.push_back(e);
        }
        cutCycles(edgesFiltradas, maxVertexId);

        if (validarCoberturaRotulos(edgesFiltradas, maxVertexId)){
            encontrouSolucaoValida = true;
            int custoAtual = rotulosEscolhidos.size();

            // Atualização das estatísticas de um determinado alfa
            somaRotulos[idxAlpha] += custoAtual;
            contagens[idxAlpha]++;

            if (custoAtual < minRotulosGlobais){
                minRotulosGlobais = custoAtual;
                melhorSolucaoGlobal = rotulosEscolhidos;
                melhorArvoreGlobal = edgesFiltradas;
            }
        }

        // Processo reativo de atualização das probabilidades
        if ((i + 1) % blockSize == 0 && encontrouSolucaoValida){
            std::vector<double> qualidade(numAlphas, 0.0);
            double somaQualidade = 0.0;

            for (int j = 0; j < numAlphas; j++){
                if(contagens[j] > 0){
                    double mediaCustos = somaRotulos[j] / contagens[j];
                    qualidade[j] = (double) minRotulosGlobais / mediaCustos;
                }
                else {
                    qualidade[j] = 0.0;
                }
                somaQualidade += qualidade[j];
            }

            // Normalização das probabilidades
            for (int j = 0; j < numAlphas; j++)
                prob[j] = (somaQualidade > 0) ? qualidade[j] / somaQualidade : 1.0 / numAlphas;
        }
    }

    std::ostringstream oss;
    if (!encontrouSolucaoValida){
        oss << "Nao foi possivel cobrir todos os vertices com os rotulos disponiveis.\n";
        return oss.str();
    }
    
    oss << "Rótulos utilizados " << minRotulosGlobais << "\n";
    oss << "Lista de rotulos escolhidos na melhor solucao: ";
    for (int r:melhorSolucaoGlobal) oss << r << " ";
    oss << "\nArestas componentes da arvore geradora:\n";
    for(const auto& e : melhorArvoreGlobal)
        oss << " (" <<e.u << " -> " << e.v << ") [Rotulo: "<< e.label << "]\n";

    return oss.str();
}