#include "EStree.hpp"
#include "MonotoneEStree.hpp"
#include "../utils/common.hpp"
#include <queue>
#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_set>

MonotoneEStree::MonotoneEStree(){}

MonotoneEStree::MonotoneEStree(int L2, int s, vector<unordered_set<pair<int, int>, PHash, PCompare>> H2)
{
    int n = H2.size();
    N.resize(n);
    L=L2;
    D=L;
    H = H2;
    p = ceil(log2(n))/2;
    to_update.resize(n, vector<int>(n));
    src = s;
    l = vector<int>(n, INF);
}

void MonotoneEStree::initialize()
{
    Dijkstra(H, src, l);
    
    for(int i=0; i<H.size(); i++)
    {
        for(auto& edge: H[i])
        {
            N[i].push({edge.second + l[edge.first], edge.first}); 
        }
    }
}

void MonotoneEStree::deleteEdge(int u, int v)
{
    increase(u, v, INF/2);
}


void MonotoneEStree::increase(int u, int v, int w)
{
    Q.push({l[u], u});
    Q.push({l[v], v});
    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> temp;
    
    N[u].changeFirst(l[v]+w, v);
    N[v].changeFirst(l[u]+w, u);
    
    updateLevels();
}

void MonotoneEStree::updateLevels()
{
    while(!Q.empty())
    {
        auto u = Q.top().second;
        Qfind.erase(u);
        Q.pop();

        auto vPair = N[u].top();
        if(vPair.second != src && vPair.second == u)
        {
            N[u].pop();
            auto t = vPair;
            vPair = N[u].top();
            N[u].push(t);
        }
        
        if(vPair.first > l[u])
        {
            l[u] = vPair.first;

            if(vPair.first > L) l[u] = INF;

            for(auto nei: H[u])
            {
                priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> temp;
    
                while(N[nei.first].pq.size()>0 && N[nei.first].top().second!=u)
                {
                    temp.push(N[nei.first].top());
                    N[nei.first].pop();
                }
                N[nei.first].pop();
                N[nei.first].push({l[u]+nei.second, u});

                
                while(!temp.empty())
                {
                    N[nei.first].push(temp.top());
                    temp.pop();
                }
                
                if(Qfind.find(nei.first) == Qfind.end())
                {
                    Q.push({l[nei.first], nei.first});
                    Qfind.insert(nei.first);
                }
            }

        }
    }
}

vector<int> MonotoneEStree::getDistances()
{
    return l;
}
