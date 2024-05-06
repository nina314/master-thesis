#include "../utils/common.hpp"
#include "KboundedMIS.hpp"
#include "../dynamicSSSP/EStree.hpp"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std;

class IncrementalAlgo {
public:
    IncrementalAlgo(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int eps);
    void insertEdge(int u, int v);
    KboundedMIS B;
    bool kBoundedRulingSet(int u, int v);
    
private:
    int i;
    vector<unordered_set<int>> L;
    vector<unordered_set<int>> S;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    int k, n;
    int r, r2;
    vector<EStree> Abig;
    vector<EStree> Asmall;
    bool initialized;
    unordered_set<int> S_union;
    unordered_set<int> V;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> H;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> Hs;
    int eps;

    unordered_set<int> sampleVertices(const unordered_set<int>& L_prev, double probability);
};

IncrementalAlgo::IncrementalAlgo(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int eps) 
    : graph(graph), k(k), i(0), eps(eps) {
    
    n = graph.size();
    for (int j = 0; j < n; j++)
        V.insert(j);
        
    r2 = 1;

    do
    {
        r2 = (1+eps)*r2;

        initialized = false;
        L.clear();
        S.clear();
        Abig.clear();
        Asmall.clear();
        H.clear();

        L.push_back(V);
        L.resize(n);
        S.resize(n);
        Abig.resize(n);
        Asmall.resize(n);
        H.resize(n);
        
    } while(!kBoundedRulingSet(0, 0));

}

unordered_set<int> IncrementalAlgo::sampleVertices(const unordered_set<int>& L_prev, double probability) 
{
    unordered_set<int> S_i;
    for (int vertex : L_prev) 
    {
        if ((double)rand() / RAND_MAX < probability) 
        {
            S_i.insert(vertex);
        }
    }
    return S_i;
}

bool IncrementalAlgo::kBoundedRulingSet(int u, int v) 
{
    if (L[i].size() > 4*k) {
        if (i == 0 || L[i].size() <= L[i - 1].size() / 2) {
            i++;
            double gamma = L[i - 1].size() / (2 * k) - 1;
            double probability = min(10.0 * log(graph.size()) / gamma, 1.0);

            if (S.size() <= i)
                S.resize(i+1);
            
            S[i] = sampleVertices(L[i-1], probability);

            unordered_set<int> S_i;

            for (int j = 1; j <= i; ++j) 
            {
                S_i.insert(S[j].begin(), S[j].end());
            }

            vector<int> centers;
            for (auto c : S_i)
                centers.push_back(c);

            auto connected_graph = buildGraphWithSources(graph, centers);
            Abig[i] = EStree(connected_graph, centers[0]);

            auto dists = Abig[i].getDistances();
            for (int j = 0; j < graph.size(); j++) 
            {
                if (dists[j] > r2)
                    L[i].insert(j);
            }
            return kBoundedRulingSet(u, v);
        } 
        else 
        {
            cout<<"there is k+1 \n";
            return false;
        }
    } 
    else 
    {
        if (!initialized) 
        {
            int d = i;
            S_union = L[d];
            for (int j = 1; j <= d; ++j) 
            {
                S_union.insert(S[j].begin(), S[j].end());
            }

            Hs.clear();
            Hs.resize(graph.size());

            for (auto s : S_union) 
            {
                Asmall[s] = EStree(graph, s);
                auto dists = Asmall[s].getDistances();

                for (int j = 0; j < dists.size(); j++) 
                {
                    if (dists[j] < r2) {
                        Hs[i].insert({j, dists[j]});
                        H[i].insert({j, dists[j]});
                    }
                }
            }

            B = KboundedMIS(Hs, k); //to implement fast version!!
            initialized = true;
        } 
        else 
        {
            for (auto s : S_union) 
            {
                Asmall[s].addEdge(u, v);
                auto dists = Asmall[s].getDistances();
                for (int j = s; j < dists.size(); j++) 
                {
                    if (dists[j] < r2) 
                    {
                        Hs[i].insert({j, dists[j]});
                        Hs[j].insert({i, dists[j]});
                        B.addEdge(i, j);
                    }
                }
            }
        }
    }
    cout<<"there is no k+1 \n";
    return true;
}

void IncrementalAlgo::insertEdge(int u, int v) 
{
    graph[u].insert({v, 1});
    graph[v].insert({u, 1});

    Abig[i].addEdge(u, v);
    auto dists = Abig[i].getDistances();

    for (auto it = L[i].begin(); it != L[i].end();) 
    {
        auto l = *it;
        if (dists[l] < r2) 
        {
            it = L[i].erase(it);
        } 
        else 
        {
            ++it;
        }
    }

    while(!kBoundedRulingSet(u, v))
    {
        r = (1+eps)*r;
        r2 = (1+eps)*r;
        
        initialized = false;
        L.clear();
        S.clear();
        Abig.clear();
        Asmall.clear();
        H.clear();

        L.push_back(V);
        L.resize(n);
        S.resize(n);
        Abig.resize(n);
        Asmall.resize(n);
        H.resize(n);
    }
}
//
//
//
//int main() {
//    unsigned seed = 12345;
//    mt19937 gen(seed);
//    uniform_int_distribution<int> weight_dist(2, 100);
//
//    int n = 300;
//
//    vector<unordered_set<pair<int, int>, PHash, PCompare>> adj(n);
//
//    for (int i = 0; i < n; ++i) {
//        for (int j = i + 1; j < n; ++j) {
//            int weight = weight_dist(gen);
//            adj[i].insert({j, weight});
//            adj[j].insert({i, weight});
//        }
//    }
//
//    int r = 3, k = 2;
//    vector<unordered_set<pair<int, int>, PHash, PCompare>> Gr(n);
//
//    for (int i = 0; i < n; i++) {
//        vector<int> dists(n, INF);
//         Dijkstra(adj, i, dists);
//        for (int j = 0; j < n; j++) {
//             if (dists[j] < r) {
//                 Gr[i].insert({j, dists[j]});
//             }
//        }
//    }
//
//    int epsilon = 1;
//    IncrementalAlgo ia(Gr, k, epsilon);
//
//    ia.kBoundedRulingSet(2, 1);
//    
//    for(auto mm: ia.B.mis) cout<<mm<<" ";
//    cout<<endl;
//    
//    ia.insertEdge(1, 5);
//    return 0;
//}
