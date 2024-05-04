#ifndef INCREMENTALDYNAMICSSSP_HPP
#define INCREMENTALDYNAMICSSSP_HPP

#include "D.hpp"
#include "Dsource.hpp"
#include "../utils/common.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

using namespace std;

class IncrementalDynamicSSSP {
public:
    vector<unordered_set<int>> A, U;
    vector<D> d;
    double epsilon;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    vector<unordered_set<int>> W;
    vector<unordered_map<int, int>> n, r;
    int k, dMax, m;

    IncrementalDynamicSSSP(int k, double eps, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int m, int dMax);
    pair<int, int> getDepthForI(int i);
    void insert(int v1, int v2, int weight);
    int query(int u, int v);
};

#endif // INCREMENTALDYNAMICSSSP_HPP
