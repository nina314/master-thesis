#include "ScaledEStree.hpp"
#include "EStree.hpp"
#include "../utils/common.hpp"
#include <queue>
#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_set>

ScaledEStree::ScaledEStree(){}

ScaledEStree::ScaledEStree(int s, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph_or)
{
    graph = graph_or;
    graph.push_back({{s, 0}});
    int n = graph.size();
    
    graph[s].insert({n-1, 0});
    
    N.resize(n);
    src = n-1;
    l = vector<int>(n, INF);
    
    Dijkstra(graph, src, l);
    
    for(int i=0; i<graph.size(); i++)
    {
        for(auto& edge: graph[i])
        {
            N[i].push({edge.second + l[edge.first], edge.first}); 
        }
    }
}

void ScaledEStree::deleteEdge(int u, int v)
{
    increase(u, v, INF/2);
    increase(v, u, INF/2);
    
    graph[u].erase({v, 1});
    graph[v].erase({u, 1});
}

void ScaledEStree::increase(int u, int v, int w)
{
    graph[u].erase({v, 1});
    graph[v].erase({u, 1});
    
    graph[u].insert({v, w});
    graph[v].insert({u, w});
    
    scan(u, v);
}

void ScaledEStree::scan(int u, int v)
{
    int w=INF/2;

    if(graph[u].find({v, 1})!=graph[u].end())
        w = (graph[u].find({v, 1}))->second;
    
    N[v].changeFirst(l[u]+w, u);
    
    if(N[v].top().first==-1) return;
    auto v2 = N[v].top().second;
    
    if(graph[v].find({v2, 1})!=graph[v].end())
        w = (graph[v].find({v2, 1}))->second;
    else w =INF/2;
    
    if(l[v2] + w > r)
        l[v] = INF;
    
    
    if(l[v2]+w > l[v])
    {
        l[v] = l[v2]+w;
        for(auto [nei, wei]: graph[v])
        {
            scan(v, nei);
        }
    }   
}

vector<int> ScaledEStree::getDistances()
{
    return l;
}

//int main() {
//    
//    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph = {
//        {{1, 10}, {2, 35}, {3, 100}}, // Neighbors of vertex 0
//        {{0, 10}, {2, 15}, {3, 5}},            // Neighbors of vertex 1
//        {{0, 35}, {1, 15}},            // Neighbors of vertex 2
//        {{0, 100}, {1, 5}}                     // Neighbors of vertex 3
//    };
//
//    ScaledEStree estree(0, graph); 
//
//    estree.getDistances();
//    
//    estree.deleteEdge(0, 1);
//    estree.getDistances();
//    
//    estree.deleteEdge(1, 2);
//    estree.getDistances();
//
//    return 0;
//}
