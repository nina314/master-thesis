#include "../utils/common.hpp"
#include <unordered_set>
#include <vector>

class FullyDynamicAlgo
{
    public:
    int s;
    unordered_set<int> centers;
    int k;
    vector<int> dists;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    
    FullyDynamicAlgo(vector<unordered_set<pair<int, int>, PHash, PCompare>> g, int k2)
    {   
        k= k2;
        s= g.size();
        g.push_back({});
        graph = g;
        centers = simulateGonzales();
    }
    
    unordered_set<int> simulateGonzales()
    {
        dists= vector<int>(graph.size(), INF);
        
        unordered_set<int> C;
        
        for(int i=0; i<k; i++)
        {
            Dijkstra(graph, s, dists);
            auto maxi = max_element(dists.begin(), dists.end());
            int c_next = distance(dists.begin(), maxi); 
            
            graph[s].insert({c_next, 0});
            graph[c_next].insert({s, 0});
            
            C.insert(c_next);
        }
        
        for(auto c: C)
        {          
            graph[s].erase({c, 0});
            graph[c].erase({s, 0});
        }
        return C;
    }
    
    void addEdge(int u, int v, int w)
    {
        graph[u].insert({v, w});
        graph[v].insert({u, w});
        centers = simulateGonzales(); 
    }
    
    void deleteEdge(int u, int v)
    {
        graph[u].erase({v, 1});
        graph[v].erase({u, 1});
        centers = simulateGonzales(); 
    }
    
    vector<int> getCenters()
    {
        vector<int> res;

        for (auto cen : centers)
            res.push_back(cen);
        
        return res;
    }

};

