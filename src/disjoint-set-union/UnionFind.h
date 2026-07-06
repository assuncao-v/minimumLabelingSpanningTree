#include <iostream>
#include <vector>
#include <string>
#include <numeric>

// Classe Union-Find (Disjoint Set Union)
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n);
    int find(int x);
    bool unite(int x, int y);
};