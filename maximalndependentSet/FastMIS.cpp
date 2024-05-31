#include "FastMIS.hpp"
#include "../utils/common.hpp"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm> 
#include <cmath> 
#include <queue> 
#include <random>

using namespace std;

vector<int> FastMIS::generateRandomPermutation(int n) {
    vector<int> permutation(n);
    for (int i = 0; i < n; ++i) {
        permutation[i] = i;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(permutation.begin(), permutation.end(), g);
    return permutation;
}

int FastMIS::getLevel(int z)
{
    return floor(log2(pi[z]+1)); 
}

vector<set<pair<int, int>, CustomCompare>> FastMIS::convertToSet(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& unorderedSets) {

    vector<set<pair<int, int>, CustomCompare>> sets(pi.size(), set<pair<int, int>, CustomCompare>(CustomCompare(pi)));

    for (int i=0; i<unorderedSets.size(); i++) {
        
        for (const auto& p : unorderedSets[i]) {
            sets[i].insert(p);
        }
    }
    return sets;
}

FastMIS::FastMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, unordered_set<int> nodes) 
{
    pi = generateRandomPermutation(graph.size());
    G = convertToSet(graph);
    int levels = ceil(log2(graph.size()+1))+1;
    V.resize(levels);
    mis = set<int, CustomCompare>(pi); 
    VV = set<int, CustomCompare>(pi); 
    
    
    for(int i=0; i<graph.size(); i++) VV.insert(i);
    
    mis = set<int, CustomCompare>(pi); 
    int i=0, f=0;
    
    set<int, CustomCompare> notDominated(pi);
        
    for(auto nn: nodes) 
    {
        notDominated.insert(nn);
    }
    
    for (auto v: VV) {
        if (!nodes.empty() && !nodes.count(v)) continue;
        
        if(notDominated.count(v))
        {
            mis.insert(v);
            
            notDominated.erase(v);
            for(auto u: G[v])
            {
                if(notDominated.count(u.first))
                notDominated.erase(u.first);
            }
        }
        
        i++;
        
        if(i==pow(2, f)  || i == VV.size())
        {
            V[f] = notDominated;
            f++;
        }
    }
}

set<int, CustomCompare> FastMIS::GreedyMIS(unordered_set<int> nodes) {
    auto new_mis = set<int, CustomCompare>(pi); 
    
    for (auto v: new_mis) {
        bool dominated = false;

        for (auto& u : G[v]) { 
            if (new_mis.count(u.first)) {
                dominated = true;
                break;
            }
        }

        if (!dominated) 
        {
            new_mis.insert(v);
            mis.insert(v);
        }
        else
        {
            mis.erase(v);
        }
    }
    

    return mis;
}

unordered_set<int> FastMIS::findInfluencedSet(int u, int v, int b) {

    unordered_set<int> S;
    priority_queue<pair<int, int> > T;

    if (!mis.count(u) || (mis.count(u) && !mis.count(v))) { 
    }
    else 
    {
        T.push({-pi[v], v});
    }
    
    unordered_set<int> seen;
    while (!T.empty()) {
        int z= T.top().second;
        T.pop();

        if (mis.count(z)) 
        {
            S.insert(z);
            int k = getLevel(z);            
            if (!V[k-1].count(z)) cerr << "Something is off. \n"; 
                
            for (auto& w : G[z]) {
                if (V[k].count(w.first) && pi[w.first] > pi[z]) {
                    if(!seen.count(w.first))
                    {
                        T.push({-pi[w.first], w.first});
                        seen.insert(w.first);
                    }
                }
            }
                
        } else {
            bool flag = true;
            for (auto& w : G[z]) {
                if (!mis.count(w.first) || !V[b].count(w.first) || pi[w.first] > pi[z]) continue;

                if (!S.count(w.first)) {
                    flag = false;
                    break;
                }

            }

            if (flag) {
                S.insert(z);
                for (auto& w : G[z]) {
                    if (!mis.count(w.first) || !V[b].count(w.first) || pi[w.first] > pi[z]) continue;
                    if(!seen.count(w.first))
                    {T.push({-pi[w.first], w.first});
                    seen.insert(w.first);}
                }
            }
        }
    }

    return S;
}

void FastMIS::update(int u, int v) {
    if (pi[v] < pi[u]) swap(u, v);
    
    int a=getLevel(u);
    int b=getLevel(v);

    auto S = findInfluencedSet(u, v, b);
    mis.erase(v);
    
    if (S.empty()) 
    {    
        int index = -1;

        for (auto [wnei, wwei] : G[v]) {
            if (mis.count(wnei)) {
                index = wnei;
                break;
            }
        }

        if (index != -1) 
        {
            int doo = getLevel(index);
            doo = min(doo, (int)V.size()-1);
            
            for (int i = max(0, a); i < doo; i++) 
            {
                V[i].insert(v);
            }
        } 
        else 
        {
            for (int i = max(0, a); i < V.size(); i++) 
            { 
                V[i].insert(v);
            }
        }
        
    } 
    else {
        for (int i = a; i < V.size(); i++) 
        {
            V[i].erase(v);
        }

        auto temp = S;
        temp.erase(v);

        auto before = mis;
        
        G[u].insert({v, 1});
        G[v].insert({u, 1});
        
        
        GreedyMIS(temp);

        G[u].erase({v,1});
        G[v].erase({u,1});
        fixSubgraphs(S, before); 
        G[u].insert({v, 1});
        G[v].insert({u, 1});
    }
}

void FastMIS::fixSubgraphs(const unordered_set<int>& S, const set<int, CustomCompare>& before) { 
    vector<int> ss;
    auto prev = V;
    
    for (auto s : S) 
    {
        ss.push_back(s);
    }
    
    sort(ss.begin(), ss.end(), [&](int a, int b) { return pi[a] < pi[b]; });

    for (int z : ss) 
    {
        int k = getLevel(z);
        
        if (!before.count(z) && mis.count(z)) // z joined mis
        {
            for (auto [w, wei] : G[z]) 
            {
                if (!V[k].count(w)) continue;
                for (int i = k + 1; i < V.size(); i++) V[i].erase(w);
            }
        } 
        else if (before.count(z) && !mis.count(z)) //z left mis
        {
            for (auto [w, wei] : G[z]) 
            {
                int index = -1;

                for (auto [wnei, wwei] : G[w]) {
                    
                    if (mis.count(wnei)) 
                    {
                        index = wnei;
                        break;
                    }
                }

                if (index != -1) 
                {
                    int doo = getLevel(index);
                    
                    for (int i = k; i <= doo; i++) 
                    {
                        V[i].insert(w);
                    }
                } 
                else {
                    for (int i = k; i < V.size(); i++) { 
                        V[i].insert(w);
                    }
                }
            }
        }
    }
}

void FastMIS::printAll()
{
    cout<<"MIS: \n";
    for(auto m: mis) cout<<m<<" ";
    cout<<endl;
}