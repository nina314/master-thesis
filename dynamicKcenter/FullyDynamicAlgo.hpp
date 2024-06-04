#include "../utils/common.hpp"
#include "../dynamicSSSP/FastDijkstra.hpp"
#include <unordered_set>
#include <vector>
#include <iostream>
#include <algorithm>

class FullyDynamicAlgo
{
public:
    int s;
    unordered_set<int> centers;
    int k, first;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    FastDijkstra fd;

    FullyDynamicAlgo(vector<unordered_set<pair<int, int>, PHash, PCompare>> g, int k2)
        : k(k2), graph(g)
    {
        s = g.size();
        graph.resize(s + 1);

        first = rand() % s;
        graph[s].insert({first, 0});
        graph[first].insert({s, 0});

        fd = FastDijkstra(graph, s);

        centers = simulateGonzales();
    }

    unordered_set<int> simulateGonzales()
    {
        auto temp = fd;
        
        unordered_set<int> C;
        C.insert(first);

        while (C.size() < k)
        {
            int r = C.size();
            auto maxi = max_element(temp.dists.begin(), temp.dists.end());
            int c_next = distance(temp.dists.begin(), maxi); 

            
            C.insert(c_next);
            if(C.size()==r) break;
            
            temp.addEdge(s, c_next, 0);
        }

        
        return C;
    }

    void addEdge(int u, int v, int w)
    {
        fd.addEdge(u, v, w);
        centers = simulateGonzales(); 
    }

    void deleteEdge(int u, int v)
    {
        graph[u].erase({v, 1});
        graph[v].erase({u, 1});

        graph[s].erase({first, 0});
        graph[first].erase({s, 0});

        first = rand() % (graph.size() - 1);

        graph[s].insert({first, 0});
        graph[first].insert({s, 0});

        fd = FastDijkstra(graph, s);

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