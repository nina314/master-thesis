#include "KboundedMIS.hpp"
#include <algorithm>

using namespace std;

KboundedMIS::KboundedMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k) : graph(graph), k(k) {
    
    degrees.resize(graph.size());
    for(int i=0; i<graph.size(); i++)
    {
        degrees[i]=graph[i].size();
        vertices.insert(i);
    }
}


unordered_set<int> KboundedMIS::update(int u, int v) {
    if (u == v || graph[u].find({v, 1}) != graph[u].end() || graph[v].find({u, 1}) != graph[u].end()) {
        return mis;
    }

    graph[u].insert({u, v});
    graph[v].insert({v, u});
    
    for(int i=0; i<graph.size(); i++)
    {
        degrees[i]=graph[i].size();
        vertices.insert(i);
    }
    
    mis.clear();
    
    while (!vertices.empty()) {
        auto minElementIt = min_element(degrees.begin(), degrees.end());
        int now = distance(degrees.begin(), minElementIt);
        if (degrees[now]>graph.size()) break;
        mis.insert(now);

        for (auto [neighbor, weight] : graph[now]) {
            degrees[neighbor] = INF / 2; 
            if (degrees[now]<graph.size()) {
                degrees[neighbor] = INF / 2; 

                for (auto [neighborOfNeighbor, w] : graph[neighbor]) {
                    if (degrees[neighborOfNeighbor] < graph.size()) {
                        degrees[neighborOfNeighbor]--;
                    }
                }
            }
        }
        degrees[now] = INF / 2; 
    }
    
    return mis;
}