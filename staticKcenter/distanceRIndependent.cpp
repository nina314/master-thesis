#include "../utils/common.hpp"
#include "distanceRIndependent.hpp"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

vector<int> maximalDistanceRIndependentSet(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int r, int k) {
    int n = graph.size();
    vector<int> independentSet;
    unordered_set<int> candidateSet;
    vector<int> distance(n, INF);

    for (int i = 0; i < n; ++i) {
        candidateSet.insert(i);
    }

//    while (!candidateSet.empty()) {
//        int v = *next(candidateSet.begin(), rand() % candidateSet.size());
//        int v = *(candidateSet.begin());
//        candidateSet.erase(v);
//
//        if (distances[v] > r) {
//            independentSet.push_back(v);
//            Dijkstra(graph, v, distances);
//        }
//    }
    
    while (!candidateSet.empty())
    {
        int start = *(candidateSet.begin());
        independentSet.push_back(start);
        
        if(independentSet.size()>k) return independentSet;
        
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        distance[start] = 0;
        pq.push({0, start});
        vector<bool> visited(distance.size(), false);
        
        while (!pq.empty()) {
            auto [du, uId] = pq.top();
            pq.pop();
            if(visited[uId]) continue;
            visited[uId] = true;
            if(distance[uId] < 2*r) candidateSet.erase(uId);

            for (auto& e : graph[uId]) {
                int v = e.first;
                int w = e.second;
                if (distance[v] > du + w) {
                    distance[v] = du + w;
                    pq.push({distance[v], v});
                }
            }
        }   
    }
    
    return independentSet;
}

vector<int> distanceRIndependent(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int maxWeight) {
    int lo = 1, hi = graph.size();
    vector<int> independentSet, res;
        
    
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        independentSet = maximalDistanceRIndependentSet(graph, mid, k);
        
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
