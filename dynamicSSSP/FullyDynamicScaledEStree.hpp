#ifndef SCALED_ES_DYN
#define SCALED_ES_DYN

#include "EStree.hpp"
#include "../utils/common.hpp"
#include <vector>
#include <map>
#include <unordered_set>

class FullyDynamicScaledEStree {
private:
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    vector<int> l;
    int src;
    vector<unordered_map<int, int>> N;
    int r = INF;
    void scan(int u, int v);
    void scan2(int u, int v);

public:
    FullyDynamicScaledEStree();
    FullyDynamicScaledEStree(int s, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph);

    void deleteEdge(int u, int v);
    void addEdge(int u, int v, int w);
    void increase(int u, int v, int w);
    vector<int> getDistances();
};

#endif
