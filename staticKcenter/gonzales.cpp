#include "../utils/common.hpp"
#include "gonzales.hpp"
#include <queue>
#include <iostream>
#include <unordered_set>

using namespace std;

void augmentedDijkstra(vector<unordered_set<pair<int, int>, PHash>>& graph, int start, vector<int>& distance, vector<unordered_set<int>>& buckets, int alpha) {
    int n = graph.size();
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    distance[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [du, uId] = pq.top();
        pq.pop();
        
        for (auto& e : graph[uId]) {
            int v = e.first;
            int w = e.second;
            if (distance[v] > du + w) {
                int bucketIndex = max(0, min((int)(log(du + w) / log(alpha)), (int)buckets.size() - 1)); 
                buckets[bucketIndex].erase(v);
                distance[v] = du + w;
                int newBucketIndex = max(0, min((int)(log(distance[v]) / log(alpha)), (int)buckets.size() - 1)); 
                buckets[newBucketIndex].insert(v);
                pq.push({distance[v], v});
            }
        }
    }
}

vector<int> gonzalezAlpha(vector<unordered_set<pair<int, int>, PHash>>& graph, int k, int alpha, int maxWeight) {
    int n = graph.size();
    int B = ceil(log(n * maxWeight) / log(alpha));

    vector<int> distance(n, INF);
    vector<unordered_set<int>> buckets(B, unordered_set<int>());

    for (int i = 0; i < n; ++i) {
        buckets[B - 1].insert(i);
    }

    vector<int> solution;
    int b_max = B - 1;
    
    while (solution.size() < k && b_max >= 0) {
        while (b_max >= 0 && buckets[b_max].empty()) {
            b_max--;
        }
        
        if (b_max >= 0) { 
            int randIndex = rand() % buckets[b_max].size();
            auto it = buckets[b_max].begin();
            advance(it, randIndex);
            int v = *it;

            solution.push_back(v);
            buckets[b_max].erase(v);

            augmentedDijkstra(graph, v, distance, buckets, alpha);
        }
        else break;
    }

    return solution;
}
