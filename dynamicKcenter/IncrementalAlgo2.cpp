#include "../utils/common.hpp"
#include "../maximalndependentSet/FastMIS.hpp"
#include "../dynamicSSSP/Dsource.hpp"
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
    FastMIS B;
    bool kBoundedRulingSet(int u, int v);
    
private:
    int i;
    vector<unordered_set<int>> L;
    vector<unordered_set<int>> S;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    int k, n;
    int r, r2;
    vector<Dsource> Abig;
    vector<Dsource> Asmall;
    bool initialized;
    unordered_set<int> S_union;
    unordered_set<int> V;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> H;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> Hs;
    int eps;

    vector<int> getCenters();
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
    if (L[i].size() > 4*k) 
    {
        if (i == 0 || L[i].size() <= L[i - 1].size() / 2) 
        {
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
            Abig[i] = Dsource(eps, INF, centers[0], connected_graph, INF);
            auto dists = Abig[i].distance;
            for (int j = 0; j < graph.size(); j++) 
            {
                if (dists[j] > r2)
                    L[i].insert(j);
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
                Asmall[s] = Dsource(eps, INF, s, graph, INF);
                auto dists = Asmall[s].distance;

                for (int j = 0; j < dists.size(); j++) 
                {
                    if (dists[j] < r2) {
                        Hs[s].insert({j, dists[j]});
                        H[s].insert({j, dists[j]});
                    }
                }
            }
                
            B = FastMIS(H, S_union); 
            initialized = true;
            return B.mis.size()>k;
        } 
        else 
        {
            for (auto s : S_union) 
            {
                Asmall[s].addEdge(u, v, 1);
                auto dists = Asmall[s].distance;
                
                for (int j = s; j < dists.size(); j++) 
                {
                    if (dists[j] < r2) 
                    {
                        Hs[i].insert({j, dists[j]});
                        Hs[j].insert({i, dists[j]});
                        B.update(i, j);
                    }
                }
            }
            return B.mis.size()>k;
        }
    }
    return B.mis.size()>k;
}

void IncrementalAlgo::insertEdge(int u, int v) 
{
    graph[u].insert({v, 1});
    graph[v].insert({u, 1});

    if(Abig[i].distance.size()>0) 
    {
        Abig[i].addEdge(u, v, 1);

        auto dists = Abig[i].distance;

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
    }
    
    while(!kBoundedRulingSet(u, v))
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
    }
}

vector<int> IncrementalAlgo::getCenters() 
{
    vector<int> res;
    
    for(auto mm: B.mis) res.push_back(mm);
    return res;
}

//int main() {
//    unsigned seed = 12345;
//    mt19937 gen(seed);
//    uniform_int_distribution<int> weight_dist(10, 50);
//
//    int n = 61;
//
//    vector<unordered_set<pair<int, int>, PHash, PCompare>> adj(n);
//
//    for (int i = 0; i < n; ++i) {
//        for (int j = i + 1; j < n; ++j) {
//            int weight = weight_dist(gen);
//            if(weight>10)
//            {
//                adj[i].insert({j, weight});
//                adj[j].insert({i, weight});
//            }
//        }
//    }
//
//    int k = 15;
//    
//    int eps = 1;
//    IncrementalAlgo ia(adj, k, eps);
////    for(auto mm: ia.B.mis) cout<<mm<<" ";
//    
//    ia.insertEdge(4, 3);
////    for(auto mm: ia.B.mis) cout<<mm<<" ";
//    
//    return 0;
//}
