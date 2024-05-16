#ifndef INCREMENTALALGO_HPP
#define INCREMENTALALGO_HPP

#include "../utils/common.hpp"
#include "../maximalndependentSet/FastMIS.hpp"
#include "../dynamicSSSP/ScaledEStree.hpp"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <random>

class ModifiedIncrementalAlgo {
public:
    ModifiedIncrementalAlgo(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int eps);

    void insertEdge(int u, int v, int w);
    vector<int> getCenters();

private:
    unordered_set<int> sampleVertices(const unordered_set<int>& L_prev, double probability);
    bool kBoundedRulingSet(int u, int v);

    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    vector<ScaledEStree> Asmall;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> H;

    unordered_set<int> V;
    FastMIS B;

    int n;
    int k;
    int i;
    int eps;
    double r2;

    static const int INF = numeric_limits<int>::max(); // Assuming INF is a large value
};

#endif // INCREMENTALALGO_HPP