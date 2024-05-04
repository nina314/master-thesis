#include "D.hpp"
#include <iostream>

using namespace std;

D::D(){}

D::D(double eps, int degree, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int mm)
    : epsilon(eps), maxDegree(degree), graph(graph), mm(mm) {
        
    for (int i=0; i<graph.size(); i++) {
        insertVertex(i);
    }
}

void D::insertVertex(int u) {
    Ds[u] = Dsource(epsilon, maxDegree, u, graph, mm);

    for (const auto& [kid, par] : Ds[u].parent) {
        U[kid].insert(u);
    }
}

unordered_map<int, unordered_map<int, int>> D::addEdge(int v1, int v2, int weight) {
    unordered_set<int> seen;
    unordered_map<int, unordered_map<int, int>> changed;

    for (auto u : U[v1]) {
        auto [toAdd, toRemove] = Ds[u].addEdge(v1, v2, weight);

        if (!toAdd.empty())
            changed[u] = Ds[u].parent;

        for (auto tR : toRemove) {
            if (U.count(tR) && U[tR].count(u)) {
                if (U[tR].count(u)) {
                    U[tR].erase(u);
                }
            }
        }

        for (auto tA : toAdd) {
            U[tA].insert(u);
        }

        seen.insert(u);
    }

    for (auto u : U[v2]) {
        if (seen.count(u))
            continue;
        
        auto [toAdd, toRemove] = Ds[u].addEdge(v1, v2, weight);

        if (!toAdd.empty())
            changed[u] = Ds[u].parent;
        
        for (auto tR : toRemove) {
            U[tR].erase(u);
        }

        for (auto tA : toAdd) {
            U[tA].insert(u);
        }

        seen.insert(u);
    }
    
    return changed;
}

int D::getDistance(int v1, int v2) {
    return Ds[v1].distance[v2];
}