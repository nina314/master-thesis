#include "KboundedMIS.hpp"

using namespace std;

bool KboundedMIS::isIndependent(int u, int v) {
    for (auto &edge : graph[u]) {
        if (mis.find(edge.first) != mis.end() || mis.find(edge.second) != mis.end()) return false;
    }
    for (auto &edge : graph[v]) {
        if (mis.find(edge.first) != mis.end() || mis.find(edge.second) != mis.end()) return false;
    }
    return true;
}

void KboundedMIS::updateMIS() {
    mis.clear();
    for (int i = 0; i < graph.size(); ++i) {
        bool isMis = true;
        for (auto &edge : graph[i]) {
            if (mis.find(edge.first) != mis.end() || mis.find(edge.second) != mis.end()) {
                isMis = false;
                break;
            }
        }
        if (isMis && mis.size() < k) {
            mis.insert(i);
        }
    }
}

KboundedMIS::KboundedMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k) : graph(graph), k(k) {
    updateMIS();
}

unordered_set<int> KboundedMIS::addEdge(int u, int v) {
    if (u == v || graph[u].find({u, 1}) != graph[u].end() || graph[v].find({v, 1}) != graph[v].end()) {
        return mis;
    }

    graph[u].insert({u, v});
    graph[v].insert({v, u});
    updateMIS();
    return mis;
}