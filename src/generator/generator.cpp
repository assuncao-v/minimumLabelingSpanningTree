#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

struct Aresta {
    int u, v, rotulo;
};

vector<Aresta> gerarArvoreGeradora(int numVertices, mt19937& rng) {
    vector<Aresta> arvore;

    for (int i = 2; i <= numVertices; i++) {
        uniform_int_distribution<int> dist(1, i - 1);
        int j = dist(rng);

        Aresta a;
        a.u = min(i, j);
        a.v = max(i, j);
        a.rotulo = 0;
        arvore.push_back(a);
    }

    return arvore;
}

vector<Aresta> completarGrafo(int numVertices, int numArestas, vector<Aresta> arvore, mt19937& rng) {
    set<pair<int, int>> existentes;
    for (int i = 0; i < (int)arvore.size(); i++)
        existentes.insert(make_pair(arvore[i].u, arvore[i].v));

    vector<pair<int, int>> candidatas;
    for (int u = 1; u <= numVertices; u++) {
        for (int v = u + 1; v <= numVertices; v++) {
            if (existentes.count(make_pair(u, v)) == 0)
                candidatas.push_back(make_pair(u, v));
        }
    }

    shuffle(candidatas.begin(), candidatas.end(), rng);

    int faltam = numArestas - (int)arvore.size();
    for (int i = 0; i < faltam; i++) {
        Aresta a;
        a.u = candidatas[i].first;
        a.v = candidatas[i].second;
        a.rotulo = 0;
        arvore.push_back(a);
    }

    return arvore;
}

void atribuirRotulos(vector<Aresta>& arestas, int numRotulos, mt19937& rng) {
    uniform_int_distribution<int> dist(1, numRotulos);
    for (int i = 0; i < (int)arestas.size(); i++)
        arestas[i].rotulo = dist(rng);
}

void salvarInstancia(string arquivo, int numVertices, int numRotulos, vector<Aresta>& arestas, mt19937& rng) {
    shuffle(arestas.begin(), arestas.end(), rng);

    ofstream out(arquivo);
    out << numVertices << " " << arestas.size() << " " << numRotulos << "\n";
    for (int i = 0; i < (int)arestas.size(); i++)
        out << arestas[i].u << " " << arestas[i].v << " " << arestas[i].rotulo << "\n";
}

void gerarInstancia(string arquivo, int numVertices, int numRotulos, double densidade, unsigned int seed) {
    mt19937 rng(seed);

    int numArestas = (int)(densidade * numVertices * (numVertices - 1) / 2.0);
    if (numArestas < numVertices - 1)
        numArestas = numVertices - 1;

    vector<Aresta> arvore  = gerarArvoreGeradora(numVertices, rng);
    vector<Aresta> arestas = completarGrafo(numVertices, numArestas, arvore, rng);
    atribuirRotulos(arestas, numRotulos, rng);
    salvarInstancia(arquivo, numVertices, numRotulos, arestas, rng);

    cout << "Gerado: " << arquivo
         << "  (n (vertices)=" << numVertices
         << ", m (arestas)=" << arestas.size()
         << ", l (rotulos)=" << numRotulos
         << ", d (densidade)=" << densidade << ")\n";
}

int main() {
    gerarInstancia("instancia01.txt", 100,   25, 0.2,  1);
    gerarInstancia("instancia02.txt", 100,   25, 0.2,  2);
    gerarInstancia("instancia03.txt", 100,   25, 0.5,  3);
    gerarInstancia("instancia04.txt", 100,   25, 0.5,  4);
    gerarInstancia("instancia05.txt", 100,   25, 0.8,  5);
    gerarInstancia("instancia06.txt", 200,   50, 0.2,  6);
    gerarInstancia("instancia07.txt", 200,   50, 0.2,  7);
    gerarInstancia("instancia08.txt", 200,   50, 0.5,  8);
    gerarInstancia("instancia09.txt", 200,   50, 0.5,  9);
    gerarInstancia("instancia10.txt", 200,   50, 0.8, 10);
    gerarInstancia("instancia11.txt", 500,  125, 0.2, 11);
    gerarInstancia("instancia12.txt", 500,  125, 0.2, 12);
    gerarInstancia("instancia13.txt", 500,  125, 0.5, 13);
    gerarInstancia("instancia14.txt", 500,  125, 0.5, 14);
    gerarInstancia("instancia15.txt", 500,  125, 0.8, 15);
    gerarInstancia("instancia16.txt", 1000, 250, 0.2, 16);
    gerarInstancia("instancia17.txt", 1000, 250, 0.2, 17);
    gerarInstancia("instancia18.txt", 1000, 250, 0.5, 18);
    gerarInstancia("instancia19.txt", 1000, 250, 0.5, 19);
    gerarInstancia("instancia20.txt", 1000, 250, 0.8, 20);

    return 0;
}
