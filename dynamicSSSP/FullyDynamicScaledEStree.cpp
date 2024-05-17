#include "FullyDynamicScaledEStree.hpp"
#include "EStree.hpp"
#include "../utils/common.hpp"
#include <queue>
#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_set>

FullyDynamicScaledEStree::FullyDynamicScaledEStree(){}

FullyDynamicScaledEStree::FullyDynamicScaledEStree(int s, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph_or)
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
            N[i][edge.first] = edge.second + l[edge.first]; 
        }
    }
}

void FullyDynamicScaledEStree::deleteEdge(int u, int v)
{
    if(l[u]>l[v]) swap(u, v);
    increase(u, v, INF/3);
}

void FullyDynamicScaledEStree::addEdge(int u, int v, int w)
{
    if(l[u]>l[v]) swap(u, v);
    
    graph[u].insert({v, w});
    graph[v].insert({u, w});
    
    scan2(u, v);
    N[u][v]=l[v]+w;
}

void FullyDynamicScaledEStree::scan2(int u, int v)
{
    int w=INF/3;

    if(graph[u].find({v, 1})!=graph[u].end())
        w = (graph[u].find({v, 1}))->second;
    
    N[v][u] = l[u]+w;
    
    //OVDJE!!
    vector<pair<int, int>> temp;
    
    for(auto [fi, se]: N[v]) temp.push_back({se, fi});
    sort(temp.begin(), temp.end());
    
    if(temp.empty()) return;
    auto v2 = temp[0].second;
    
    if(graph[v].find({v2, 1})!=graph[v].end())
        w = (graph[v].find({v2, 1}))->second;
    else w =INF/3;
      
    if(l[v2] + w > r)
        l[v] = INF/3;
    
    if(l[v2]+w < l[v])
    {
        l[v] = l[v2]+w;
        
        for(auto [wei, nei]: temp)
        {
            scan2(v, nei);
        }
    }
}

void FullyDynamicScaledEStree::increase(int u, int v, int w)
{
    graph[u].erase({v, 1});
    graph[v].erase({u, 1});
   
    graph[u].insert({v, w});
    graph[v].insert({u, w});
    
    scan(u, v);
}

void FullyDynamicScaledEStree::scan(int u, int v)
{
    int w=INF/3;
    
    if(graph[u].find({v, 1})!=graph[u].end())
        w = (graph[u].find({v, 1}))->second;
    
    N[v][u] = l[u]+w;
    for(auto& edge: graph[v])
    {
        N[v][edge.first] =edge.second + l[edge.first]; 
    }
    
    vector<pair<int, int>> temp;
    
    for(auto [fi, se]: N[v]) temp.push_back({se, fi});
    sort(temp.begin(), temp.end());
    
    if(temp.empty()) return;
    auto v2 = temp[0].second;
    // ovjde!!
    if(graph[v2].find({v, 1})!=graph[v2].end())
        w = (graph[v2].find({v, 1}))->second;
    else w = INF/3;
    
    if(l[v2] + w > INF/3)
        l[v] = INF/3;
    
    
    if(l[v2]+w > l[v])
    {
        l[v] = l[v2]+w;
        
        for(auto [wei, nei]: temp)
        {
            if(graph[nei].count({v, 1}))
            {
                scan(v, nei);
            }
        }
    }   
}

vector<int> FullyDynamicScaledEStree::getDistances()
{
    auto ll = l;
    ll.pop_back();
    return ll;
}