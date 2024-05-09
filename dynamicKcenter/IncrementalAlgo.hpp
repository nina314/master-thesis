#ifndef INCREMENTAL_ALGO_HPP
#define INCREMENTAL_ALGO_HPP

#include "../utils/common.hpp"
#include "../maximalndependentSet/FastMIS.hpp"
#include "../dynamicSSSP/Dsource.hpp"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std;

class IncrementalAlgo {
public:
    IncrementalAlgo(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int eps);
    void insertEdge(int u, int v);
    FastMIS B;
    bool kBoundedRulingSet(int u, int v);
    vector<int> getCenters();
    
private:
    int i;
    vector<unordered_set<int>> L;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    int k, n;
    int r, r2;
    vector<Dsource> Abig;
    vector<Dsource> Asmall;
    bool initialized;
    unordered_set<int> S_i, S_union;
    unordered_set<int> V;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> H;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> Hs;
    int eps;

    unordered_set<int> sampleVertices(const unordered_set<int>& L_prev, double probability);
};

#endif // INCREMENTAL_ALGO_HPP
