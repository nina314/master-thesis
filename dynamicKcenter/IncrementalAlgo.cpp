#include "../utils/common.hpp"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std;


class IncrementalAlgo
{
    public:
    int i; 
    vector<unordered_set<int>> L;
    vector<unordered_set<int>> S;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    int k;

    IncrementalAlgo(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k) : graph(graph), k(k)
    {
        i = 0;
        unordered_set<int> V;

        for(int j=0; j<graph.size(); j++) 
            V.insert(j);

        L.push_back(V);
        L.resize(graph.size());
        S.resize(graph.size());

    }

    unordered_set<int> sampleVertices(const unordered_set<int>& L_prev, double probability) {
        unordered_set<int> S_i;
        for (int vertex : L_prev) {
            if ((double)rand() / RAND_MAX < probability) {
                S_i.insert(vertex);
            }
        }

        return S_i;
    }

    bool kBoundedRulingSet(int u, int v) {

        if (L[i].size() > 4 * k) 
        {
            if (i == 0 || L[i].size() <= L[i - 1].size() / 2) 
            {
                i++;
                double gamma = L[i - 1].size() / (2*k) - 1;
                double probability = min(10.0 * log(graph.size()) / gamma, 1.0);

                if(S.size()<=i) S.push_back(unordered_set<int>());
                S[i] = sampleVertices(L[i - 1], probability);
                for (int x : L[i - 1]) {
                    bool is_independent = true;
                    for (int y : S[i]) 
                    {
                        if (graph[x].count({y, 1})) 
                        {
                            is_independent = false;
                            break;
                        }
                    }

                    if (is_independent) 
                    {
                        L[i].insert(x);
                    }
                }

                return kBoundedRulingSet(u, v);
            } 
            else 
            {
                return false;
            }
        }
        else
        {
            static bool initialized = false;
            static unordered_set<int> B;

            if (!initialized) 
            {
                int d = i;
                auto S_union = L[d];
                for (int j = 1; j <= d; ++j) 
                {
                    S_union.insert(S[j].begin(), S[j].end());
                }
                B.insert(S_union.begin(), S_union.end()); 
                initialized = true;

                return true;
            } 
            else if (S[i].count(u) && S[i].count(v)) 
            {
                B.insert({u, v});
            }
        }
        return true;
    }

    bool insertEdge(int u, int v) {
        graph[u].insert({v, 1});
        graph[v].insert({u, 1});

        for (int j = 0; j <= i; ++j) {
            if (L[j].count(u) && any_of(S.begin(), S.begin() + j + 1, [v](const unordered_set<int>& set) { return set.count(v); })) {
                L[j].erase(u);
            } else if (L[j].count(v) && any_of(S.begin(), S.begin() + j + 1, [u](const unordered_set<int>& set) { return set.count(u); })) {
                L[j].erase(v);
            }
        }

        return kBoundedRulingSet(u, v);
    }
};
    