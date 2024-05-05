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

public:
    ScaledEStree();
    ScaledEStree(int s, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph);

    void deleteEdge(int u, int v);
    void increase(int u, int v, int w);
    void scan(int u, int v);
    vector<int> getDistances();
};

#endif
