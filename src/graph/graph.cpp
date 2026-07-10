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
 
std::vector<int> Graph::mvca() {
    std::vector<Edge> totalEdges;
    listofEdges(*this, totalEdges);

    // Agrupa arestas por rótulo (evitando duplicatas verificando u < v)
    map<double, std::vector<Edge>> porRotulo;
    for (const auto& e : totalEdges) {
        if (e.u < e.v) porRotulo[e.label].push_back(e);
    }

    set<int> rotulosDisponiveis;
    for (const auto& par : porRotulo) {
        rotulosDisponiveis.insert(par.first);
    }

    UnionFind uf(num_vertices + 1); // +1 caso os vértices sejam 1-indexados
    int componentes = num_vertices;
    vector<int> rotulosEscolhidos;

    while (componentes > 1 && !rotulosDisponiveis.empty()) {
        double melhorRotulo = -1.0;
        int maiorReducao = -1;

        // Avalia qual rótulo reduz mais o número de componentes conexas
        for (double rotulo : rotulosDisponiveis) {
            UnionFind temp_uf = uf; // Cria uma cópia do estado atual para simulação
            int reducao = 0;

            for (const auto& e : porRotulo[rotulo]) {
                if (temp_uf.unite(e.u, e.v)) {
                    reducao++;
                }
            }

            if (reducao > maiorReducao) {
                maiorReducao = reducao;
                melhorRotulo = rotulo;
            }
        }

        // Se nenhum rótulo consegue mais conectar componentes isoladas, encerramos
        if (maiorReducao <= 0) break;

        // Aplica o melhor rótulo definitivamente no Union-Find oficial
        rotulosEscolhidos.push_back(melhorRotulo);
        rotulosDisponiveis.erase(melhorRotulo);
        
        for (const auto& e : porRotulo[melhorRotulo]) {
            if (uf.unite(e.u, e.v)) {
                componentes--;
            }
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
    UnionFind uf(num_vertices + 1); // +1 porque vertices sao 1-indexados
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
    listofEdges(*this, totalEdges); // Obtém todas as arestas do grafo original

    // Agrupa as arestas por rótulo (evitando duplicatas checando u < v)
    std::map<double, std::vector<Edge>> porRotulo;
    for (const auto& e : totalEdges) {
        if (e.u < e.v) {
            porRotulo[e.label].push_back(e);
        }
    }

    // Coleta todos os rótulos disponíveis no grafo
    std::set<double> rotulosDisponiveis;
    for (const auto& par : porRotulo) {
        rotulosDisponiveis.insert(par.first);
    }

    // Encontra o maior ID de vértice para inicializar o UnionFind com segurança
    int maxVertexId = 0;
    for (No* no : nos) {
        if (no->id > maxVertexId) {
            maxVertexId = no->id;
        }
    }

    // Inicializa o UnionFind oficial e o contador de componentes
    UnionFind uf(maxVertexId + 1); 
    int componentes = num_vertices;
    
    std::vector<double> rotulosEscolhidos;
    std::vector<Edge> arestasArvoreFinal; // Guardará as arestas da árvore geradora

    // Loop Guloso: roda até que restem apenas 1 componente (tudo conectado)
    while (componentes > 1 && !rotulosDisponiveis.empty()) {
        double melhorRotulo = -1.0;
        int maiorReducao = -1;

        // 1. Simulação: Avalia qual rótulo reduz mais o número de componentes
        for (double rotulo : rotulosDisponiveis) {
            UnionFind temp_uf = uf; // Cria uma cópia do estado atual para simular
            int reducao = 0;

            for (const auto& e : porRotulo[rotulo]) {
                if (temp_uf.unite(e.u, e.v)) {
                    reducao++;
                }
            }

            if (reducao > maiorReducao) {
                maiorReducao = reducao;
                melhorRotulo = rotulo;
            }
        }

        // Se nenhum rótulo consegue mais conectar componentes isoladas, interrompe
        if (maiorReducao <= 0) break;

        // 2. Aplicação Real: Aplica o melhor rótulo definitivamente no Union-Find oficial
        rotulosEscolhidos.push_back(melhorRotulo);
        rotulosDisponiveis.erase(melhorRotulo);
        
        for (const auto& e : porRotulo[melhorRotulo]) {
            // Se a união der certo, significa que esta aresta reduziu uma componente
            // e NÃO gerou ciclos. Portanto, ela faz parte da árvore final!
            if (uf.unite(e.u, e.v)) {
                componentes--;
                arestasArvoreFinal.push_back(e); 
            }
        }
    }

    // Monta a string de retorno formatada exatamente como o seu main.cpp espera
    std::ostringstream oss;
    if (componentes > 1) {
        oss << "Nao foi possivel cobrir todos os vertices com os rotulos disponiveis.\n";
        return oss.str();
    }

    // Primeira linha crucial para a leitura do main.cpp ("Rotulos utilizados: X")
    oss << "Rotulos utilizados: " << rotulosEscolhidos.size() << "\n";
    
    oss << "Lista de rotulos escolhidos de forma gulosa: ";
    for (double r : rotulosEscolhidos) {
        oss << r << " ";
    }
    oss << "\nArestas componentes da arvore geradora:\n";
    for (const auto& e : arestasArvoreFinal) {
        oss << "  (" << e.u << " -> " << e.v << ") [Label: " << e.label << "]\n";
    }

    return oss.str();
}

struct Candidate {
    double rotulo;
    int reducao;
};

std::string Graph::minimumLabelingSpanningTreeRandomized(double alpha) {
    std::vector<Edge> totalEdges;
    listofEdges(*this, totalEdges); // Obtém todas as arestas do grafo original

    // Agrupa as arestas por rótulo (evitando duplicatas checando u < v)
    std::map<double, std::vector<Edge>> porRotulo;
    for (const auto& e : totalEdges) {
        if (e.u < e.v) {
            porRotulo[e.label].push_back(e);
        }
    }

    // Coleta todos os rótulos disponíveis no grafo
    std::set<double> rotulosOrignais;
    for (const auto& par : porRotulo) {
        rotulosOrignais.insert(par.first);
    }

    // Encontra o maior ID de vértice para inicializar o UnionFind com segurança
    int maxVertexId = 0;
    for (No* no : nos) {
        if (no->id > maxVertexId) {
            maxVertexId = no->id;
        }
    }

    // Variaveis para guardar a MELHOR solução global
    int minRotulosGlobais = std::numeric_limits<int>::max();
    std::vector<double> melhorSolucaoGlobal;
    std::vector<Edge> melhorArvoreGlobal;
    bool encontrouSolucaoValida = false;

    // Número de iterações do laço externo
    int maxIter = 100; // Pode ser ajustado conforme necessário

    for(int iter = 0; iter < maxIter; iter++){
        // Inicializa o UnionFind oficial e o contador de componentes
        UnionFind uf(maxVertexId + 1); 
        int componentes = num_vertices;
        
        std::set<double> rotulosDisponiveis = rotulosOrignais;
        std::vector<double> rotulosEscolhidos;
        std::vector<Edge> arestasArvoreFinal; // Guardará as arestas da árvore geradora

        // Loop Guloso: roda até que restem apenas 1 componente (tudo conectado)
        while (componentes > 1 && !rotulosDisponiveis.empty()) {
            std::vector<Candidate> candidatos;
            //double melhorRotulo = -1.0;
        // int maiorReducao = -1;

            // 1. Simulação: Avalia qual rótulo reduz mais o número de componentes
            for (double rotulo : rotulosDisponiveis) {
                UnionFind temp_uf = uf; // Cria uma cópia do estado atual para simular
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

            // Se nenhum rótulo consegue reduzir componentes, interrompe
            if (candidatos.empty()) break;

            // 2. Aplicação Real: Aplica o melhor rótulo definitivamente no Union-Find oficial
            // Ordena os candidatos por redução (decrescente) e escolhe o primeiro
            std::sort(candidatos.begin(), candidatos.end(), [](const Candidate& a, const Candidate& b) {
                return a.reducao > b.reducao;
            });

            // 3. k = randomRange(0, alfa * candidatos.count() - 1);
            // Calcula o limite superior do sorteio. O std::max garante que não teremos números negativos.
            int limiteMax = std::max(0, (int)(alpha * candidatos.size()) - 1);

            // Sorteia o índice k
            int k = (limiteMax == 0) ? 0 : rand() % (limiteMax + 1);

            // 4. Elemento escolhido
            double melhorRotulo = candidatos[k].rotulo;

            rotulosEscolhidos.push_back(melhorRotulo);
            rotulosDisponiveis.erase(melhorRotulo);
            
            for (const auto& e : porRotulo[melhorRotulo]) {
                // Se a união der certo, significa que esta aresta reduziu uma componente
                // e NÃO gerou ciclos. Portanto, ela faz parte da árvore final!
                if (uf.unite(e.u, e.v)) {
                    componentes--;
                    arestasArvoreFinal.push_back(e); 
                }
            }
        } // Fim Loop Guloso

        // Se conseguiu conectar o grafo e foi melhor do que o recorde global, atualiza
        if (componentes == 1) {
            encontrouSolucaoValida = true;
            if ((int)rotulosEscolhidos.size() < minRotulosGlobais) {
                minRotulosGlobais = rotulosEscolhidos.size();
                melhorSolucaoGlobal = rotulosEscolhidos;
                melhorArvoreGlobal = arestasArvoreFinal;
            }
        }
    } // Fim do loop de iterações

    // Monta a string de retorno baseada na MELHOR solução encontrada
    std::ostringstream oss;
    if (!encontrouSolucaoValida) {
        oss << "Nao foi possivel cobrir todos os vertices com os rotulos disponiveis.\n";
        return oss.str();
    }

    // Formatação idêntica para o main.cpp ler sem quebrar
    oss << "Rotulos utilizados: " << minRotulosGlobais << "\n";
    oss << "Lista de rotulos escolhidos de forma gulosa randomizada (alpha=" << alpha << "): ";
    for (double r : melhorSolucaoGlobal) {
        oss << r << " ";
    }
    oss << "\nArestas componentes da arvore geradora:\n";
    for (const auto& e : melhorArvoreGlobal) {
        oss << "  (" << e.u << " -> " << e.v << ") [Label: " << e.label << "]\n";
    }

    return oss.str();
}






















std::string Graph::minimumLabelingSpanningTreeGRASP() {
    // Semente para a aleatoriedade do GRASP (idealmente chamada uma vez no main, mas seguro aqui)
    srand(time(NULL)); 

    std::vector<Edge> totalEdges;
    listofEdges(*this, totalEdges);

    // Agrupa as arestas por rótulo
    std::map<double, std::vector<Edge>> porRotulo;
    for (const auto& e : totalEdges) {
        if (e.u < e.v) {
            porRotulo[e.label].push_back(e);
        }
    }

    std::set<double> rotulosDisponiveisIniciais;
    for (const auto& par : porRotulo) {
        rotulosDisponiveisIniciais.insert(par.first);
    }

    int maxVertexId = 0;
    for (No* no : nos) {
        if (no->id > maxVertexId) maxVertexId = no->id;
    }

    // Variáveis globais do GRASP
    int minRotulosGlobais = std::numeric_limits<int>::max();
    std::vector<double> melhorSolucaoGlobal;
    std::vector<Edge> melhorArvoreGlobal;
    
    int maxIteracoes = 100; // Quantas vezes o GRASP vai rodar
    double alpha = 0.8;     // Fator de aleatoriedade (0.0 a 1.0)

    // ==========================================
    // LOOP PRINCIPAL DO GRASP
    // ==========================================
    for (int iteracao = 0; iteracao < maxIteracoes; iteracao++) {
        UnionFind uf(maxVertexId + 1);
        int componentes = num_vertices;
        
        std::set<double> rotulosDisponiveis = rotulosDisponiveisIniciais;
        std::vector<double> rotulosEscolhidos;
        std::vector<Edge> arestasArvore;

        // --- FASE 1: Construção Gulosa Randomizada ---
        while (componentes > 1 && !rotulosDisponiveis.empty()) {
            int maiorReducao = -1;
            std::map<double, int> reducaoDoRotulo;

            // Avalia o potencial de todos os rótulos
            for (double rotulo : rotulosDisponiveis) {
                UnionFind temp_uf = uf;
                int reducao = 0;
                for (const auto& e : porRotulo[rotulo]) {
                    if (temp_uf.unite(e.u, e.v)) reducao++;
                }
                reducaoDoRotulo[rotulo] = reducao;
                if (reducao > maiorReducao) maiorReducao = reducao;
            }

            if (maiorReducao <= 0) break;

            // Monta a Lista Restrita de Candidatos (RCL)
            std::vector<double> rcl;
            for (double rotulo : rotulosDisponiveis) {
                if (reducaoDoRotulo[rotulo] >= alpha * maiorReducao) {
                    rcl.push_back(rotulo);
                }
            }

            // Escolhe um rótulo aleatório da RCL
            int randomIndex = rand() % rcl.size();
            double melhorRotulo = rcl[randomIndex];

            // Aplica no grafo atual
            rotulosEscolhidos.push_back(melhorRotulo);
            rotulosDisponiveis.erase(melhorRotulo);
            
            for (const auto& e : porRotulo[melhorRotulo]) {
                if (uf.unite(e.u, e.v)) {
                    componentes--;
                    arestasArvore.push_back(e);
                }
            }
        }

        // --- FASE 2: Busca Local (Drop Heuristic) ---
        // Tenta remover rótulos que ficaram redundantes
        for (size_t i = 0; i < rotulosEscolhidos.size(); i++) {
            double rotuloTeste = rotulosEscolhidos[i];
            
            // Simula o grafo SEM esse rótulo
            UnionFind teste_uf(maxVertexId + 1);
            int compTeste = num_vertices;
            
            for (double r : rotulosEscolhidos) {
                if (r == rotuloTeste) continue; // Pula o rótulo que estamos testando
                for (const auto& e : porRotulo[r]) {
                    if (teste_uf.unite(e.u, e.v)) compTeste--;
                }
            }
            
            // Se o grafo continuou conectado (1 componente), o rótulo é inútil!
            if (compTeste == 1) {
                rotulosEscolhidos.erase(rotulosEscolhidos.begin() + i);
                i--; // Ajusta o índice pois o vetor diminuiu
                
                // Precisamos reconstruir as arestas da árvore sem ele
                arestasArvore.clear();
                UnionFind refaz_uf(maxVertexId + 1);
                for (double r : rotulosEscolhidos) {
                    for (const auto& e : porRotulo[r]) {
                        if (refaz_uf.unite(e.u, e.v)) arestasArvore.push_back(e);
                    }
                }
            }
        }

        // --- ATUALIZA O MELHOR GLOBAL ---
        if (componentes == 1 && rotulosEscolhidos.size() < minRotulosGlobais) {
            minRotulosGlobais = rotulosEscolhidos.size();
            melhorSolucaoGlobal = rotulosEscolhidos;
            melhorArvoreGlobal = arestasArvore;
        }
    }

    // ==========================================
    // FORMATAÇÃO DA SAÍDA
    // ==========================================
    std::ostringstream oss;
    if (minRotulosGlobais == std::numeric_limits<int>::max()) {
        oss << "Nao foi possivel cobrir todos os vertices com os rotulos disponiveis.\n";
        return oss.str();
    }

    oss << "Rotulos utilizados: " << minRotulosGlobais << "\n";
    oss << "Lista de rotulos (GRASP): ";
    for (double r : melhorSolucaoGlobal) {
        oss << r << " ";
    }
    oss << "\nArestas da arvore:\n";
    for (const auto& e : melhorArvoreGlobal) {
        oss << "  (" << e.u << " -> " << e.v << ") [Label: " << e.label << "]\n";
    }

    return oss.str();
}
