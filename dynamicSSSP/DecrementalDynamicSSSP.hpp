#ifndef DECREMENTAL
#define DECREMENTAL

#include "../utils/common.hpp"
#include "MonotoneEStree.hpp"
#include "EStree.hpp"
#include <tuple>
#include <vector>
#include <unordered_set>

class DecrementalDynamicSSSP
{
    public:
    int n; 
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> F;
    vector<vector<int>> matrix;
    vector<int> priorities; 
    vector<int> distances; 
    vector<EStree> trees_from_sets;
    vector<EStree> trees_from_nodes;
    MonotoneEStree mes;
    int p; 
    int m;
    int D;
    int eps; 
    int phi;
    
    DecrementalDynamicSSSP(vector<unordered_set<pair<int, int>, PHash, PCompare>> &graph, int D, int eps, int src);
    void initialize();
    vector<int> getDistances();
    void deleteEdge(int s, int d);
};

#endif
