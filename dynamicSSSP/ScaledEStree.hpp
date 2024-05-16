#ifndef SCALED_ES
#define SCALED_ES

#include "EStree.hpp"
#include "../utils/DynamicHeap.hpp"
#include "../utils/common.hpp"
#include <vector>
#include <unordered_set>

class ScaledEStree {
private:
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    vector<int> l;
    int src;
    vector<DynamicHeap> N;
    int r = INF;
    void scan(int u, int v);
    void scan2(int u, int v);

public:
    ScaledEStree();
    ScaledEStree(int s, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph);

    void deleteEdge(int u, int v);
    void addEdge(int u, int v, int w);
    void increase(int u, int v, int w);
    vector<int> getDistances();
};

#endif
