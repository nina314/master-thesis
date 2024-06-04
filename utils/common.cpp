#include "common.hpp"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <chrono>
#include <functional>
#include <fstream>

void Dijkstra(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int start, vector<int>& distance) {
    int n = graph.size();
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    distance[start] = 0;
    pq.push({0, start});
    vector<bool> visited(distance.size(), false);
    
    while (!pq.empty()) {
        auto [du, uId] = pq.top();
        pq.pop();
        if(visited[uId]) continue;
        visited[uId] = true;
        
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

vector<unordered_set<pair<int, int>, PHash, PCompare>> buildGraphWithSources(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, vector<int> centers)
{
    int source = centers[0];
    for(auto v: centers)
    {
        if(v == source) continue;
        
        graph[v].insert({source, 0});
        graph[source].insert({v, 0});
    }
    
    return graph;
}


int cost(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, vector<int> centers)
{
    if(centers.empty()) return -1;
    int source = centers[0];
    graph = buildGraphWithSources(graph, centers);
    vector<int> distance(graph.size(), INF);
    
    Dijkstra(graph, source, distance);
    auto max_dist = max_element(distance.begin(), distance.end());
    return *max_dist;
}

vector<unordered_set<pair<int, int>, PHash, PCompare>> getGraph(string name, int nodes)
{
    vector<unordered_set<pair<int, int>, PHash, PCompare>> adj;
    ifstream inputFile(name);
    string a, b, c ="1";
    
    while(inputFile>>a>>b>>c)
    {
        int a1 = stoi(a), b1 = stoi(b), c1 = stoi(c);
        
        if(a1>nodes || b1>nodes) continue;
        
        if (adj.size() <= max(a1, b1)) {
            adj.resize(max(a1, b1) + 1);
        }
        
        adj[a1].insert({b1,c1});
        adj[b1].insert({a1,c1});
    }
    
    for(int i=0; i<adj.size(); i++)
    {
        if(adj[i].empty()) adj.erase(adj.begin()+i);
    }
    
    cout<<"Graph imported \n";
    return adj;
}

vector<pair<int, pair<int, int>>> getQueries(string name)
{
    vector<pair<int, pair<int, int>>> ques;
    ifstream inputFile(name);
    string a, b, c;
    
    while(inputFile>>a>>b>>c)
    {
        int a1 = stoi(a), b1 = stoi(b), c1 = stoi(c);
        ques.push_back({a1, {b1, c1}});
    }
    
    return ques;
}