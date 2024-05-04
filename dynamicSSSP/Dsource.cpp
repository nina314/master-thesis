#include "Dsource.hpp"
#include <queue>
#include <limits>
#include <cmath>
#include <iostream>

using namespace std;

Dsource::Dsource(double eps, int degree, int src, vector<unordered_set<pair<int, int>, PHash, PCompare>> g, int mmax)
    : epsilon(eps), maxDegree(degree), source(src), m(10), mm(mmax) {

    for(int i=0; i<g.size(); i++)
    {
        for(auto [nei, wei]: g[i])
        {
            graph[i][nei] = wei;
        }
    }
        
    for (const auto& kvp : graph) {
        distance.push_back(numeric_limits<int>::max()/3);
        parent[kvp.first] = -1;
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    unordered_map<int, int> distance_temp;
        
    unordered_map<int, bool> visited;
    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        distance[u] = distance_temp[u];
        
        if(visited[u]) continue;
        visited[u] = true;
            
        m += graph[u].size();
//        if (m > mm) break; // Don't want too many edges

        for (const auto& kvp : graph[u]) {
            int v = kvp.first;
            int weight = kvp.second;

            if (distance[v] > distance[u] + weight) {
                if (distance_temp[u] + weight < maxDegree) {
                    distance_temp[v] = distance_temp[u] + weight;
                    parent[v] = u;
                    pq.push({distance_temp[v], v});
                }
            }
        }
    }
    
    if(!pq.empty())
    {
        auto dist = pq.top().first;
        int power = floor(log(dist) / log(1 + epsilon));
        maxDegree = pow(1 + epsilon, power); // Adapt maxDegree
    }

    for (auto it = parent.begin(); it != parent.end();) {
        auto [kid, par] = *it;

        if (distance[kid] > maxDegree) {
            m -= graph[kid].size();
            it = parent.erase(it);

        } else {
            ++it;
        }
    }
}

pair<unordered_set<int>, unordered_set<int>> Dsource::addEdge(int u, int v, int weight) {
    graph[u][v] = weight;
    graph[v][u] = weight;

    unordered_set<int> toAdd;
    unordered_set<int> toRemove;

    if (distance[u] + weight >= distance[v] && distance[v] + weight >= distance[u])
        return {toAdd, toRemove};
   auto distance_temp = distance;
    
    priority_queue<std::pair<int, int>, vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    if (distance[u] + weight < distance[v])
        pq.push({distance[u], u});
    else
        pq.push({distance[v], v});

    unordered_map<int, bool> visited;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if(visited[u]) continue;
        visited[u] = true;
        
        distance[u] = distance_temp[u];

        if (parent[u] != -1 && u != source) {
            m += graph[u].size();
//            if (m > mm) break;
        }

        for (const auto& kvp : graph[u]) {
            int v = kvp.first;
            int weight = kvp.second;

            if (distance_temp[v] > distance_temp[u] + weight) {
                if (distance_temp[u] + weight < maxDegree) {
                    distance_temp[v] = distance_temp[u] + weight;
                    if (!parent.count(v))
                        toAdd.insert(v);
                    parent[v] = u;
                    pq.push({distance_temp[v], v});
                }
            }
        }
    }

        if(!pq.empty())
        {
            auto dist = pq.top().first;
            int power = floor(log(dist) / log(1 + epsilon));
            maxDegree = pow(1 + epsilon, power);
        }
        
        for (auto it = parent.begin(); it != parent.end();) {
            auto [kid, par] = *it;

            if (distance[kid] > maxDegree) {
                m -= graph[kid].size();
                if (toAdd.find(kid)!=toAdd.end())
                    toAdd.erase(kid);
                else
                {
                    toRemove.insert(kid);
                }
                
                it = parent.erase(it);
            } else {
                ++it;
            }
        }
    
    return {toAdd, toRemove};
}