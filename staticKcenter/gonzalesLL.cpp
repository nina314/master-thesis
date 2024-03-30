#include "../utils/common.hpp"
#include "gonzales.hpp"
#include <queue>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void augmentedDijkstra(vector<unordered_set<pair<int, int>, PHash>>& graph, int start, vector<int>& distance, vector<int>& node_buckets, int alpha) {
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
                int duw = du + w;
                if (duw > 0) { // Check if du + w is positive
                    int bucketIndex = max(0, min((int)(log(duw) / log(alpha)), n - 1)); 
                    node_buckets[v] = bucketIndex;
                    distance[v] = duw;
                    pq.push({distance[v], v});
                }
            }
        }
    }
}

vector<int> gonzalezAlpha(vector<unordered_set<pair<int, int>, PHash>>& graph, int k, int alpha, int maxWeight) {
    int n = graph.size();
    int B = ceil(log(n * maxWeight) / log(alpha));
    
    vector<int> distance(n, INF);
    vector<int> node_buckets(n, B - 1);

    vector<int> solution;

    while (solution.size() < k) {
        
        vector<int> maxis;
        int maxi = -1; // Initialize maxi with a negative value
        
        for(int i = 0; i < node_buckets.size(); i++) {
            if(node_buckets[i] == maxi) {
                maxis.push_back(i);
            }
            else if(node_buckets[i] > maxi) {
                maxi = node_buckets[i];
                maxis = {i};
            }
        }
        int v = maxis[rand() % maxis.size()];
        node_buckets[v] = 0;
        solution.push_back(v);
        
        augmentedDijkstra(graph, v, distance, node_buckets, alpha);
    }

    return solution;
}
