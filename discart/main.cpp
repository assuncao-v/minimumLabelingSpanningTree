#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "MeuGrafo.hpp"

void testAll(); // declarada em src/testAll.cpp

int main(int argc, char *argv[]){
    if (argc < 2) {
        std::cerr << "Uso: ./GraphProject <arquivo.txt>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << argv[1] << "\n";
        return 1;
    }

    std::string line;
    bool dir;
    int nodeCount;

    std::getline(file, line);
    dir = (line == "1");

    MeuGrafo grafo(dir);

    std::getline(file, line);
    nodeCount = std::stoi(line);

    for (int i = 0; i < nodeCount; i++) {
        getline(file, line); // lê o ID do vértice (pode ser ignorado se for sequencial)
        grafo.inserirVertice(line.empty() ? i : std::stoi(line)); // insere o vértice usando o ID lido ou o índice se a linha estiver vazia
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue; // ignora linhas vazias
        std::istringstream iss(line);
        int u, v;
        double weight;
        if (!(iss >> u >> v >> weight)) {
            std::cerr << "Formato de linha inválido: " << line << "\n";
            continue; // pula linhas mal formatadas
        }
        grafo.inserirAresta(u, v, weight); // adiciona a aresta com base no tipo de grafo
    }

    while (true){
        std::cout << "Digite um comando (ou 'sair' para encerrar): \n";
        std::cout << "AGM por rotulos\n";
        std::cout << "sair - Encerra o programa\n";
        std::cout << "Comando: ";
        std::string command;
        std::getline(std::cin, command);
        if (command == "AGM por rotulos") {
            std::cout << endl;
            std::cout << "Árvore Geradora Mínima do grafo:\n";
            std::cout << grafo.minimumLabelingSpanningTree() << "\n";
            std::cout << endl;
        } else if (command == "sair") {
            std::cout << "Encerrando o programa...\n";
            return 0;
        } else {
            std::cout << "Comando desconhecido. Tente novamente.\n";
        }
        // Aqui você pode adicionar mais comandos para interagir com o grafo
    }

    return 0;
}