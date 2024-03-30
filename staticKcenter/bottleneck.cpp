#include "bottleneck.hpp"
#include "../utils/common.hpp"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <iostream>

using namespace std;

vector<int> bottleneckFixed(vector<unordered_set<pair<int, int>, PHash>> graph, int r) {
    int n = graph.size();
    vector<int> independentSet;
    vector<int> distance(n, INF);
    unordered_set<int> candidateSet;
    
    for (int i = 0; i < n; ++i) {
        candidateSet.insert(i);
    }
    
    int source = *next(candidateSet.begin(), rand() % candidateSet.size());
    candidateSet.erase(source);
    independentSet.push_back(source);

    while (!candidateSet.empty()) {
        
        Dijkstra(graph, source, distance);
        
        auto it = candidateSet.begin(); 

        while (it != candidateSet.end()) {
            if (distance[*it] < 2 * r) {
                it = candidateSet.erase(it);
            } else {
                ++it; 
            }
        }
        
        if(candidateSet.empty()) break;
            
        int v = *next(candidateSet.begin(), rand() % candidateSet.size());
        candidateSet.erase(v);
        independentSet.push_back(v);
        
        source = v;
    }
    return independentSet;
}

vector<int> bottleneck(vector<unordered_set<pair<int, int>, PHash>>& graph, int k, int maxWeight) {
    int lo = 0, hi = maxWeight*graph.size();
    vector<int> independentSet, res;
    int cost;
        
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        independentSet = bottleneckFixed(graph, mid);
        
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
