#include "KboundedMIS.hpp"

using namespace std;

KboundedMIS::KboundedMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k) : graph(graph), k(k) {
    
    for(int i=0; i<graph.size(); i++)
    {
        degrees.push({graph[i].size(), i});
        vertices.insert(i);
    }
}


unordered_set<int> KboundedMIS::update(int u, int v) {
    if (u == v || graph[u].find({v, 1}) != graph[u].end() || graph[v].find({u, 1}) != graph[u].end()) {
        return mis;
    }

    graph[u].insert({u, v});
    graph[v].insert({v, u});
    
    degrees.mapa[u]++;
    degrees.mapa[v]++;
    
    mis.clear();
    auto Q = degrees;
    auto W = vertices;
    
    while(!W.empty() && mis.size()<k+1)
    {
        int now = Q.top().second;
        if(W.find(now)==W.end())
        {
            Q.pop();
            continue;
        }
        
        W.erase(now);
        mis.insert(now);
        
        for(auto ed: graph[now])
        {
            W.erase(ed.first);
        }
    }
    
    return mis;
}