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

vector<set<pair<int, int>, CustomCompare>> FastMIS::convertToSet(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& unorderedSets) {

    vector<set<pair<int, int>, CustomCompare>> sets(pi.size(), set<pair<int, int>, CustomCompare>(CustomCompare(pi)));

    for (int i=0; i<unorderedSets.size(); i++) {
        
        for (const auto& p : unorderedSets[i]) {
            sets[i].insert(p);
        }
    }
    return sets;
}

FastMIS::FastMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, unordered_set<int> nodes) {
    pi = generateRandomPermutation(graph.size());
    G = convertToSet(graph);
    int levels = ceil(log2(graph.size()))+1;
    V.resize(levels);
    mis = set<int, CustomCompare>(pi); 
    VV = set<int, CustomCompare>(pi); 
    
    
    for(int i=0; i<graph.size(); i++) VV.insert(i);
    
    GreedyMIS(nodes);
    
}

set<int, CustomCompare> FastMIS::GreedyMIS(unordered_set<int> nodes) {
    mis.clear();
    int i=0, f=0;
    
    for (auto v: VV) {
        if (!nodes.empty() && !nodes.count(v)) continue;
        bool dominated = false;

        for (auto& u : G[v]) { 
            if (nodes.empty() || nodes.count(u.first)) {
                if (mis.count(u.first)) {
                    dominated = true;
                    break;
                }
            }
        }

        if (!dominated) 
        {
            mis.insert(v);
        }
        
        
        i++;
        
        if(i==pow(2, f)  || i == VV.size())
        {
            V[f] = VV;
            
            for(auto m: mis)
            {
                for(auto [nei, wei]: G[m]) 
                {
                    if (nodes.empty() || nodes.count(nei))
                    {   
                        V[f].erase(nei);
                    }
                }
                V[f].erase(m);
            }
            f++;
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
            int k;
            
            if(pi[z]!=0)
                k = floor(log2(pi[z]));                
                
            if (!V[k].count(z)) cout << "Something is off. \n"; 
                
            for (auto& w : G[z]) {
                if (V[k].count(w.first) && pi[w.first] > pi[z]) {
                    if(!seen.count(w.first))
                    {T.push({-pi[w.first], w.first});
                    seen.insert(w.first);}
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
    
    
    int a=-1;
    
    if(pi[u]!=0)
        a = floor(log2(pi[u]));

    int b = floor(log2(pi[v]));

    auto S = findInfluencedSet(u, v, b);
    
        
    if (S.empty()) {
        int index = -1;

        for (auto [wnei, wwei] : G[v]) {
            if (mis.count(wwei)) {
                index = wwei;
                break;
            }
        }

        if (index != -1) {
            int doo = floor(log2(index));
            for (int i = a; i <= doo; i++) {
                V[i].insert(v);
            }
        } else {
            for (int i = a; i < V.size(); i++) { 
                V[i].insert(v);
            }
        }
        
    } else {
        for (int i = a + 1; i < V.size(); i++) 
            V[i].erase(v);

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

    for (int z : ss) {
        
        int k;
        
        if(pi[z]!=0)
            k = floor(log2(pi[z]));
        else continue;
        
        if (!before.count(z) && mis.count(z)) // z joined mis
        {
            for (auto [w, wei] : G[z]) {
                if (!V[k].count(w)) continue;
                for (int i = k + 1; i < V.size(); i++) V[i].erase(w);
            }
        } else if (before.count(z) && !mis.count(z)) {
            for (auto [w, wei] : G[z]) {
                int index = -1;

                for (auto [wnei, wwei] : G[w]) {
                    if (mis.count(wwei)) {
                        index = wwei;
                        break;
                    }
                }

                if (index != -1) {
                    int doo = floor(log2(index));
                    for (int i = k; i <= doo; i++) {
                        V[i].insert(w);
                    }
                } else {
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
    
//        for(int i=0; i<V.size(); i++)
//        {
//            cout<<"V["<<i<<"]: \n";
//            for(auto v: V[i]) cout<<v<<" ";
//            cout<<endl;
//        }
}

//int main() {
//    
//    vector<int> order = {1, 4, 2, 0, 3, 5};
//    
//    vector<set<pair<int, int>, CustomCompare>> graph(order.size(), set<pair<int, int>, CustomCompare>(CustomCompare(order)));
//
//    graph[0] = {{2, 1}};
//    graph[1] = {{5, 1}, {3, 1}};
//    graph[2] = {{0, 1}, {2, 1}};
//    graph[3] = {{2, 1}, {1, 1}};
//    graph[4] = {};
//    graph[5] = {{1, 1}};
//    
//    FastMIS fastMIS(graph, order);
//    fastMIS.printAll();
//    
//    cout<<"upd1 \n";
//    fastMIS.update(3, 4);
//    fastMIS.printAll();
//    
//    cout<<"upd2 \n";
//    fastMIS.update(1, 4);
//    fastMIS.printAll();
//    
//    cout<<"upd3 \n";
//    fastMIS.update(5, 4);
//    fastMIS.printAll();
//        
//    cout<<"upd4 \n";
//    fastMIS.update(2, 5);
//    fastMIS.printAll();
//    
//    cout<<"upd5 \n";
//    fastMIS.update(3, 5);
//    fastMIS.printAll();
//    
//    cout<<"upd6 \n";
//    fastMIS.update(0, 5);
//    fastMIS.printAll();
//    
//    return 0;
//}
