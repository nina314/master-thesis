#ifndef INCREMENTAL
#define INCREMENTAL

#include "../utils/common.hpp"
#include "EStree.hpp"
#include <vector>
#include <unordered_set>

class IncrementalDynamicSSSP
{
    public:
    int n; 
    vector<unordered_set<pair<int, int>, PHash>> graph;
    vector<unordered_set<pair<int, int>, PHash>> F;
    vector<vector<int>> matrix;
    vector<int> priorities; 
    vector<int> distances; 
    vector<EStree> trees_from_sets;
    vector<EStree> trees_from_nodes;
    int p; 
    int m;
    int D;
    int eps; 
    int phi;
    
    IncrementalDynamicSSSPR(vector<unordered_set<pair<int, int>, PHash>> &graph, int D, int eps, int src);
    void initialize();
    vector<int> getDistances();
    void addEdge(int s, int d);
};

#endif
