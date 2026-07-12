#include "../../include/Graph.hpp"
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <climits>

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

int extrairRotulos(const string& resultado) {
    size_t pos = resultado.find("Rotulos utilizados: ");
    if (pos == string::npos) return -1;
    pos += 20;
    return stoi(resultado.substr(pos));
}

double extrairAlpha(const string& resultado) {
    size_t pos = resultado.find("Melhor alpha: ");
    if (pos == string::npos) return -1.0;
    pos += 14;
    return stod(resultado.substr(pos));
}

// ============================================================
// Experimento 1: MVCA guloso (1 execucao por instancia)
// ============================================================
void experimento1(int numInstancias) {
    cout << "\n";
    cout << string(60, '=') << "\n";
    cout << "  EXPERIMENTO 1 — MVCA Guloso (1 execucao por instancia)\n";
    cout << string(60, '=') << "\n\n";
    cout << left
         << setw(18) << "Instancia"
         << setw(12) << "Rotulos"
         << setw(14) << "Tempo (ms)"
         << "\n";
    cout << string(44, '-') << "\n";

    for (int i = 1; i <= numInstancias; i++) {
        string numero = (i < 10 ? "0" : "") + to_string(i);
        string arquivo = "instancias/instancia" + numero + ".txt";

        int numVertices, numArestas, numRotulos;
        Graph g = lerInstancia(arquivo, numVertices, numArestas, numRotulos);

        auto t1 = chrono::high_resolution_clock::now();
        string resultado = g.minimumLabelingSpanningTree();
        auto t2 = chrono::high_resolution_clock::now();
        double tempoMs = chrono::duration<double, milli>(t2 - t1).count();

        int rotulos = extrairRotulos(resultado);

        cout << left
             << setw(18) << ("instancia" + numero)
             << setw(12) << rotulos
             << setw(14) << fixed << setprecision(3) << tempoMs
             << "\n";
    }
}

// ============================================================
// Experimento 2: MVCA Randomizado (30 execucoes por alpha)
// ============================================================
void experimento2(int numInstancias) {
    int numIter = 30;
    double alphas[5] = {0.05, 0.10, 0.15, 0.30, 0.50};

    cout << "\n";
    cout << string(60, '=') << "\n";
    cout << "  EXPERIMENTO 2 — MVCA Randomizado\n";
    cout << "  " << numIter << " execucoes por alpha | alphas: {0.05, 0.10, 0.15, 0.30, 0.50}\n";
    cout << string(60, '=') << "\n";

    for (int i = 1; i <= numInstancias; i++) {
        string numero = (i < 10 ? "0" : "") + to_string(i);
        string arquivo = "instancias/instancia" + numero + ".txt";

        int numVertices, numArestas, numRotulos;
        Graph g = lerInstancia(arquivo, numVertices, numArestas, numRotulos);

        cout << "\n" << string(60, '-') << "\n";
        cout << "  Instancia: instancia" << numero
             << "  |  n=" << numVertices
             << "  m=" << numArestas
             << "  l=" << numRotulos << "\n";
        cout << string(60, '-') << "\n";

        cout << left
             << setw(8)  << "Alpha"
             << setw(10) << "Exec"
             << setw(12) << "Rotulos"
             << setw(14) << "Tempo (ms)"
             << setw(12) << "Media rot."
             << setw(14) << "Media tempo"
             << "\n";
        cout << string(70, '-') << "\n";

        for (double a : alphas) {
            double somaRotulos = 0;
            double somaTempos = 0;

            for (int exec = 1; exec <= numIter; exec++) {
                auto t1 = chrono::high_resolution_clock::now();
                string resultado = g.minimumLabelingSpanningTreeRandomized(a);
                auto t2 = chrono::high_resolution_clock::now();
                double tempoMs = chrono::duration<double, milli>(t2 - t1).count();

                int rotulos = extrairRotulos(resultado);
                somaRotulos += rotulos;
                somaTempos  += tempoMs;

                cout << left
                     << setw(8)  << fixed << setprecision(2) << a
                     << setw(10) << exec
                     << setw(12) << rotulos
                     << setw(14) << setprecision(3) << tempoMs
                     << "\n";
            }

            double mediaRotulos = somaRotulos / numIter;
            double mediaTempo   = somaTempos  / numIter;

            cout << string(70, '-') << "\n";
            cout << "  Media (alpha=" << fixed << setprecision(2) << a << "): "
                 << "rotulos=" << setprecision(2) << mediaRotulos
                 << "  tempo=" << setprecision(3) << mediaTempo << "ms\n";
            cout << string(70, '-') << "\n";
        }
    }
}

// ============================================================
// Experimento 3: GRASP Reativo (30 execucoes por instancia)
// ============================================================
void experimento3(int numInstancias) {
    int numIter = 30;

    cout << "\n";
    cout << string(60, '=') << "\n";
    cout << "  EXPERIMENTO 3 — GRASP Reativo (" << numIter << " execucoes por instancia)\n";
    cout << string(60, '=') << "\n";

    // Tabelas de resultados
    vector<string>  nomes(numInstancias);
    vector<double>  mediaRotulos(numInstancias);
    vector<double>  mediaTempo(numInstancias);
    vector<int>     melhorRotulos(numInstancias);
    vector<double>  melhorTempos(numInstancias);

    for (int i = 1; i <= numInstancias; i++) {
        string numero = (i < 10 ? "0" : "") + to_string(i);
        string arquivo = "instancias/instancia" + numero + ".txt";
        nomes[i-1] = "instancia" + numero;

        int numVertices, numArestas, numRotulos;
        Graph g = lerInstancia(arquivo, numVertices, numArestas, numRotulos);

        cout << "\n" << string(60, '-') << "\n";
        cout << "  Instancia: instancia" << numero
             << "  |  n=" << numVertices
             << "  m=" << numArestas
             << "  l=" << numRotulos << "\n";
        cout << string(60, '-') << "\n";
        cout << left
             << setw(8)  << "Exec"
             << setw(12) << "Rotulos"
             << setw(12) << "Melhor alpha"
             << setw(14) << "Tempo (ms)"
             << "\n";
        cout << string(46, '-') << "\n";

        double somaRot = 0, somaTmp = 0;
        int melhor = INT_MAX;

        for (int exec = 1; exec <= numIter; exec++) {
            auto t1 = chrono::high_resolution_clock::now();
            string resultado = g.minimumLabelingSpanningTreeGRASP();
            auto t2 = chrono::high_resolution_clock::now();
            double tempoMs = chrono::duration<double, milli>(t2 - t1).count();

            int rotulos    = extrairRotulos(resultado);
            double alpha   = extrairAlpha(resultado);

            somaRot += rotulos;
            somaTmp += tempoMs;
            if (rotulos < melhor) melhor = rotulos;

            cout << left
                 << setw(8)  << exec
                 << setw(12) << rotulos
                 << setw(12) << fixed << setprecision(2) << alpha
                 << setw(14) << setprecision(3) << tempoMs
                 << "\n";
        }

        mediaRotulos[i-1] = somaRot / numIter;
        mediaTempo[i-1]   = somaTmp / numIter;
        melhorRotulos[i-1] = melhor;

        cout << string(46, '-') << "\n";
        cout << "  Media: rotulos=" << fixed << setprecision(2) << mediaRotulos[i-1]
             << "  tempo=" << setprecision(3) << mediaTempo[i-1] << "ms"
             << "  melhor=" << melhor << "\n";
    }

    // Tabela de qualidade
    cout << "\n" << string(60, '=') << "\n";
    cout << "  TABELA DE QUALIDADE — GRASP Reativo\n";
    cout << string(60, '=') << "\n";
    cout << left
         << setw(16) << "Instancia"
         << setw(14) << "Media rotulos"
         << setw(14) << "Melhor"
         << "\n";
    cout << string(44, '-') << "\n";
    for (int i = 0; i < numInstancias; i++) {
        cout << left
             << setw(16) << nomes[i]
             << setw(14) << fixed << setprecision(2) << mediaRotulos[i]
             << setw(14) << melhorRotulos[i]
             << "\n";
    }

    // Tabela de tempo
    cout << "\n" << string(60, '=') << "\n";
    cout << "  TABELA DE TEMPO — GRASP Reativo\n";
    cout << string(60, '=') << "\n";
    cout << left
         << setw(16) << "Instancia"
         << setw(18) << "Media tempo (ms)"
         << "\n";
    cout << string(34, '-') << "\n";
    for (int i = 0; i < numInstancias; i++) {
        cout << left
             << setw(16) << nomes[i]
             << setw(18) << fixed << setprecision(3) << mediaTempo[i]
             << "\n";
    }
}

int main() {
    int numInstancias = 20;

    experimento1(numInstancias);
    experimento2(numInstancias);
    experimento3(numInstancias);

    return 0;
}
