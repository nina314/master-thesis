#include "DecrementalAlgo.hpp"
#include "../utils/common.hpp"
#include <unordered_set>
#include <iostream>

DecrementalAlgo::DecrementalAlgo(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int epss, int kk)
{
    originalGraph = graph;
    eps = epss;
    k = kk;
//    lo = 1;
    r = 1;
}

vector<int> DecrementalAlgo::initialize()
{
//    hi = ceil(log2(originalGraph.size()) / log2(eps+1));
//    while(lo < hi)
//    {
//        int mid = floor((lo+hi)/2);
//        
//        auto res = tryInitialize(pow(1+eps, mid), pow(1+eps, mid+1));
//        
//        if(res)
//        {
//            r = pow(1+eps, mid);
//            hi = mid;
//        }
//        else lo = mid+1;
//    }
//    
//    r2 = hi*(1+eps);
    
    while(!tryInitialize(r, r*(1+eps)))
    {
        r = r*(1+eps);
    }
    
    r2 = r*(1+eps);
    
    return M;
}

bool DecrementalAlgo::tryInitialize(int r, int r2)
{   
    vector<int> distances(originalGraph.size(), INF);
    
    M.clear();
    
    int s = 0;

    while(s<originalGraph.size())
    {
        M.push_back(s);
        Dijkstra(originalGraph, s, distances);
        
        if(M.size()>k) 
        {
            return false;
        }

        while(s<originalGraph.size() && distances[s] <= r) s++;
    }
    
    int source = M[0];
    connectedGraph = buildGraphWithSources(originalGraph, M);
    es = EStree(connectedGraph, source);

    return true;
}

bool DecrementalAlgo::tryDeleteEdge(int s, int d)
{
    es.deleteEdge(s, d);
    vector<int> distances = es.getDistances();
    bool needsRestarting;
    
    for(int i=0; i<distances.size(); i++)
    {
        if(distances[i]> r2)
        {
            needsRestarting = true;
            M.push_back(i);
            if(M.size()>k) return false;
            
            connectedGraph[M[0]].insert({i, 0});
            connectedGraph[i].insert({M[0], 0});
        }
    }
    
    if(needsRestarting)
        es = EStree(connectedGraph, M[0]);
    
    return true;
}

vector<int> DecrementalAlgo::getCenters()
{
    return M;
}

vector<int> DecrementalAlgo::deleteEdge(int s, int d)
{
    originalGraph[s].erase({d, 1});
    originalGraph[d].erase({s, 1});
    
    if(tryDeleteEdge(s, d)) return M;
//    lo++;
    r = r*(1+eps);
    return initialize();
}
