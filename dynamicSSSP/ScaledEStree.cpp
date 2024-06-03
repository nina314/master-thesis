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
    l = vector<int>(n, INF/3);
    
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
    if(l[u]>l[v]) swap(u, v);
//    increase(u, v, INF/3);
    increase(u, v, INF/3);
    
//    graph[u].erase({v, 1});
//    graph[v].erase({u, 1});
}

void ScaledEStree::addEdge(int u, int v, int w)
{
    if(l[u]>l[v]) swap(u, v);
    
    graph[u].insert({v, w});
    graph[v].insert({u, w});
    
    scan2(u, v);
    N[u].push({l[v]+w, v});
}

void ScaledEStree::scan2(int u, int v)
{
    int w=INF/3;

    if(graph[u].find({v, 1})!=graph[u].end())
        w = (graph[u].find({v, 1}))->second;
    
    N[v].push({l[u]+w, u});
    
    
    if(N[v].top().first==-1) return;
    auto v2 = N[v].top().second;
    
    if(graph[v].find({v2, 1})!=graph[v].end())
        w = (graph[v].find({v2, 1}))->second;
    else w =INF/3;
      
    if(l[v2] + w > r)
        l[v] = INF/3;
    
    if(l[v2]+w < l[v])
    {
        l[v] = l[v2]+w;
        
        for(auto [nei, wei]: graph[v])
        {
            scan2(v, nei);
        }
    }
}

void ScaledEStree::increase(int u, int v, int w)
{
    graph[u].erase({v, 1});
    graph[v].erase({u, 1});
   
    graph[u].insert({v, w});
    graph[v].insert({u, w});
    
    visited.clear();
    scan(u, v);
}

void ScaledEStree::scan(int u, int v)
{
    int w=INF/3;
    if(visited.find(u)!=visited.end()) return;
    visited.insert(u);
    
    if(graph[u].find({v, 1})!=graph[u].end())
        w = (graph[u].find({v, 1}))->second;
    
    N[v].changeFirst(l[u]+w, u);
    for(auto& edge: graph[v])
    {
        N[v].changeFirst(edge.second + l[edge.first], edge.first); 
    }
    
    if(N[v].top().first==-1) return;
    auto v2 = N[v].top().second;
    
    if(graph[v2].find({v, 1})!=graph[v2].end())
        w = (graph[v2].find({v, 1}))->second;
    else w = INF/3;
    
    if(l[v2] + w > INF/3)
        l[v] = INF/3;
    
    
    if(l[v2]+w > l[v])
    {
        l[v] = l[v2]+w;
        
        auto t = N[v];
        
        
        while(!t.pq.empty())
        {
            auto nei = t.top().second;
            t.pop();
            if(graph[nei].count({v, 1}))
            {
                scan(v, nei);
            }
        }
    }   
}

vector<int> ScaledEStree::getDistances()
{
    auto ll = l;
    ll.pop_back();
    return ll;
}