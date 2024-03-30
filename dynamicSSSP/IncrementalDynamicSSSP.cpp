#include "IncrementalDynamicSSSP.hpp"

#include "../utils/common.hpp"
#include "DecrementalDynamicSSSP.hpp"
#include "MonotoneEStree.hpp"
#include "EStree.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <algorithm>

IncrementalDynamicSSSP::IncrementalDynamicSSSP(vector<unordered_set<pair<int, int>, PHash>> &graph, int D, int eps, int src)
: graph(graph), D(D), n(graph.size()), eps(eps)
{
    m = 0;
    
    for (auto &edges : graph) {
        m += edges.size();
    }
    m /= 2;
    
    int log_n = log2(n);
    p = ceil(log_n)/2;

    priorities.resize(n, 0);
    distances.resize(n);
    matrix.resize(p+2, vector<int>(n, INF));
    F.resize(n);
    trees_from_nodes.resize(n);
    trees_from_sets.resize(p+2);

    phi = eps*floor(sqrt(n))/(p+1);
    phi = max(phi, 1);

    initialize();
}

void IncrementalDynamicSSSP::initialize() {
    vector<unordered_set<int>> sets(p+2);
    
    for (int u = 0; u < n; ++u) {
            sets[0].insert(u);
        }
        
        for (int i = 1; i <p; i++) {
            for (auto &edges : graph) {
                for (auto &edge : edges) {
                    
                    if (rand() / static_cast<int>(RAND_MAX) < 1.0 / (pow(m, i / p))) {
                        sets[i].insert(edge.first);
                        sets[i].insert(edge.second);
                        
                        priorities[edge.first] = i;
                        priorities[edge.second] = i;
                    }
                }
            }
        }
        for (int i = 1; i < p; i++) {
            
            auto vec = vector<int>(sets[i].begin(), sets[i].end());
            auto temp = buildGraphWithSources(graph, vec);
            trees_from_sets[i] = EStree(temp, vec[0]);
            matrix[i] = trees_from_sets[i].getDistances();
        }
    
        for(int u = 0; u<n; u++)
        {
            auto maxDepth = matrix[priorities[u]+1][u];
            trees_from_nodes[u] = EStree(graph, u, maxDepth);

            auto dists = trees_from_nodes[u].getDistances();

            for(int i=0; i<dists.size(); i++)
            {
                if(dists[i]>maxDepth) continue;
                
                auto newValue = dists[i] - dists[i] % phi;
                F[u].insert({i, newValue});
            }
        }   
    
    return;
    }

void IncrementalDynamicSSSP::addEdge(int s, int d) {
        auto prev = matrix;
        for (int i = 1; i < p; i++) {
            trees_from_sets[i].deleteEdge(s, d, D); 
            matrix[i] = trees_from_sets[i].getDistances();
        }
        vector<unordered_map<int, int>> F2(n);
        
        for(int u = 0; u<n; u++)
        {
            int potDepth = pow(2, floor(log(matrix[priorities[u]+1][u])));
            int prevDepth = pow(2, floor(log(prev[priorities[u]+1][u])));
            auto maxDepth = min(D, potDepth);
            if(potDepth > prevDepth) 
                trees_from_nodes[u] = EStree(graph, u, maxDepth);
            
            trees_from_nodes[u].deleteEdge(s, d, maxDepth);
            auto dists = trees_from_nodes[u].getDistances();
            
            for(int i=0; i<dists.size(); i++)
            {
                if(dists[i]>maxDepth) continue;
                
                auto newValue = dists[i] - dists[i] % phi;
                
                F2[u][i]= newValue;
            }
        }
    
        for(int i=0; i<F.size(); i++)
        {
            for(auto& edge: F[i])
            {
                if(F2[i].find(edge.first) == F2[i].end())
                {
                    mes.deleteEdge(i, edge.first);
                }
                else if(F2[i][edge.first] > edge.second)
                {
                    mes.increase(i, edge.first, F2[i][edge.first]);
                }
            }
        }     
    }
                

    vector<int> IncrementalDynamicSSSP::getDistances()
    {
        return mes.getDistances();
    }