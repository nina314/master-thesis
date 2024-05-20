#include "GreedyMIS.hpp"

using namespace std;

GreedyMIS::GreedyMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>> G, int k) : graph(G), k(k)
{
    for(int i=0; i<graph.size(); i++) VV.insert(i);
    mis.clear();
    
    for (auto v: VV) {
        bool dominated = false;
        
        for(auto mm: mis)
        {
            if(G[v].find({mm, 1})!=G[v].end())
            {
                dominated = true;
                break;
            }
        }

        if (!dominated) 
        {
            mis.insert(v);
        }
    }
}


unordered_set<int> GreedyMIS::update(int u, int v) {
    if (u == v || graph[u].find({v, 1}) != graph[u].end() || graph[v].find({u, 1}) != graph[u].end()) {
        return mis;
    }

    graph[u].insert({u, v});
    graph[v].insert({v, u});
    mis.clear();
    
    for (auto vv: VV) {
        bool dominated = false;
        
        for(auto mm: mis)
        {
            if(graph[vv].find({mm, 1})!=graph[vv].end())
            {
                dominated = true;
                break;
            }
        }

        if (!dominated) 
        {
            mis.insert(vv);
        }
    }
    
    return mis;
}