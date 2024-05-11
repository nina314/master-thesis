#include "EStree.hpp"
#include <tuple>
#include <vector>
#include <queue>
#include <iostream>
#include <unordered_set>

using namespace std;

EStree::EStree(){}

EStree::EStree(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int src, int maxDepth)
{
    alpha.resize(graph.size());
    beta.resize(graph.size());
    gamma.resize(graph.size());
    
    distances.resize(graph.size(), INF);
    
    source = src;
    queue<int> q;

    distances[source] = 0;
    q.push(source);
    while(!q.empty())
    {
        auto current = q.front();
        q.pop();
        
        if(distances[current]>maxDepth)
        {
            continue;
        }
        
        for(auto [nei, x]: graph[current])
        {
            if(distances[nei] == distances[current])
            {
                if(nei!=current)
                    beta[nei].insert(current);
            }
            else if(distances[nei]-1> distances[current])
            {
                distances[nei] = distances[current]+1;
                
                if(distances[nei]>maxDepth)
                {
                    continue;
                }
                
                q.push(nei);
                alpha[nei].insert(current);
                gamma[current].insert(nei);
            }
            else if(distances[nei]-1== distances[current])
            {
                alpha[nei].insert(current);
                gamma[current].insert(nei);
            }
        }   
    }
}

void EStree::deleteEdge(int s, int d, int maxDepth)
{
    if(distances[s] == distances[d])
    {
        beta[s].erase(d);
        beta[d].erase(s);
    }

    if(distances[s] > distances[d]) swap(s, d);

    alpha[d].erase(s);
    gamma[s].erase(d);

    if(!alpha[d].empty()) return;

    queue<int> q;
    q.push(d);
    int k=0;
    
    while(!q.empty())
    {
        k++;
        auto w = q.front();
        q.pop();

        if(distances[w]>maxDepth) continue;
        
        distances[w]++;
        
        
        for(auto nei: beta[w])
        {
            beta[nei].erase(w);
            if(distances[w]<=maxDepth) gamma[nei].insert(w);
        }
        
        alpha[w] = beta[w];
        for(auto nei: gamma[w])
        {
            alpha[nei].erase(w);
            if(distances[w]<=maxDepth) beta[nei].insert(w);

            if(alpha[nei].empty())
            {
                q.push(nei);
            }
        }
        beta[w] = gamma[w];
        beta[w].erase(w);
        gamma[w].clear();
        if(distances[w]>maxDepth)
        {
            alpha[w].clear();
            beta[w].clear();
        }
        
        if(alpha[w].empty() && distances[w]<=maxDepth)
        {
            q.push(w);
        }
        gamma[w].erase(w);
        alpha[w].erase(w);
        
    }
}

void EStree::addEdge(int s, int d) 
{
    if (distances[s] == distances[d]) {
        beta[s].insert(d);
        beta[d].insert(s);
    }

    if (distances[s] > distances[d]) swap(s, d);

    alpha[d].insert(s);
    gamma[s].insert(d);

    distances[d] = distances[s]+1;
    
    queue<int> q;
    q.push(d);

    while (!q.empty()) {
        auto w = q.front();
        q.pop();
        
        unordered_set<int> temp;
        
        for (auto nei : beta[w]) {
            if(distances[nei]>distances[w])
            {
                beta[nei].erase(w);
                alpha[nei].insert(w);
                gamma[w].insert(nei);
            }
            else temp.insert(nei);
        }

        beta[w]=temp;
        
        for (auto nei : gamma[w]) {
            if(distances[nei]>distances[w]+1)
            {
            distances[nei] = distances[w]+1;
            q.push(nei);
            }
        }
        
        temp.clear();
        
        for (auto nei : alpha[w]) {
            if(distances[nei]<distances[w])
            {
                temp.insert(nei);
                continue;
            }
            
            gamma[nei].erase(w);
            
            if(distances[nei]== distances[w])
            {
                beta[w].insert(nei);
                beta[nei].insert(w);
            }
            else if(distances[nei]> distances[w])
            {
                distances[nei] = distances[w]+1;
                alpha[nei].insert(w);
                gamma[w].insert(nei);
                q.push(nei);
            }
        }
        alpha[w] = temp;
        
    }
}

vector<int> EStree::getDistances()
{
    return distances;
}
