#include "../utils/common.hpp"
#include "distanceRIndependent.hpp"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

vector<int> maximalDistanceRIndependentSet(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int r) {
    int n = graph.size();
    vector<int> independentSet;
    unordered_set<int> candidateSet;
    vector<int> distances(n, INF);

    for (int i = 0; i < n; ++i) {
        candidateSet.insert(i);
    }

    while (!candidateSet.empty()) {
        int v = *next(candidateSet.begin(), rand() % candidateSet.size());
        candidateSet.erase(v);

        if (distances[v] > r) {
            independentSet.push_back(v);
            Dijkstra(graph, v, distances);
        }
    }
    
    return independentSet;
}

vector<int> distanceRIndependent(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int maxWeight) {
    int lo = 0, hi = maxWeight*graph.size();
    vector<int> independentSet, res;
        
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        independentSet = maximalDistanceRIndependentSet(graph, mid);
        
        if (independentSet.size() <= k)
        {
            hi = mid-1;
            res = independentSet;
        }
        else
            lo = mid+1;
    }
    
    return res;
}
