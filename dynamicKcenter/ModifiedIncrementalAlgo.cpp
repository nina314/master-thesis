#include "ModifiedIncrementalAlgo.hpp"
#include <iostream>

using namespace std;

ModifiedIncrementalAlgo::ModifiedIncrementalAlgo(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int eps)
    : graph(graph), k(k), i(0), eps(eps) {
    n = graph.size();
    for (int j = 0; j < n; j++)
        V.insert(j);

    r2 = 1;

    do {
        i = 0;
        r2 = (1 + eps) * r2;
        Asmall.clear();
        Asmall.resize(n);

        H.clear();
        H.resize(n);

    } while (!kBoundedRulingSet(0, 0));
}

unordered_set<int> ModifiedIncrementalAlgo::sampleVertices(const unordered_set<int>& L_prev, double probability) {
    unordered_set<int> S_i;
    return S_i;
}

bool ModifiedIncrementalAlgo::kBoundedRulingSet(int u, int v) {
    for (auto s : V) {
        Asmall[s] = ScaledEStree(s, graph);
        
        auto dists = Asmall[s].getDistances();

        for (int j = 0; j < dists.size(); j++) {
            if (dists[j] < r2) {
                H[s].insert({j, dists[j]});
            }
        }
    }
    B = FastMIS(H, {});
    return B.mis.size() <= k;
}

void ModifiedIncrementalAlgo::insertEdge(int u, int v, int w) {
    graph[u].insert({v, w});
    graph[v].insert({u, w});

    for (auto s : V) {
        Asmall[s].addEdge(u, v, w);
        auto dists = Asmall[s].getDistances();

        for (int j = 0; j < dists.size(); j++) {
            if (dists[j] < r2) {
                H[s].insert({j, dists[j]});
            }
        }
    }

    B = FastMIS(H, {});

    while (B.mis.size() > k) {
        r2 *= (1 + eps);
        for (auto s : V) {
            auto dists = Asmall[s].getDistances();

            for (int j = 0; j < dists.size(); j++) {
                if (dists[j] < r2) {
                    H[s].insert({j, dists[j]});
                }
            }
        }
        B = FastMIS(H, {});
    }
}

vector<int> ModifiedIncrementalAlgo::getCenters() {
    vector<int> res;

    for (auto mm : B.mis)
        res.push_back(mm);
    return res;
}
