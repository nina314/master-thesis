#include "IncrementalAlgo.hpp"
#include <iostream>

IncrementalAlgo::IncrementalAlgo(const std::vector<std::unordered_set<std::pair<int, int>, PHash, PCompare>>& graph, int k, int eps)
    : graph(graph), k(k), i(0), eps(eps) {
    n = graph.size();
    for (int j = 0; j < n; j++)
        V.insert(j);

    r2 = 1;

    do
    {
        i = 0;
        r2 = (1 + eps) * r2;
        Asmall.clear();
        Asmall.resize(n);

        H.clear();
        H.resize(n);

    } while (!kBoundedRulingSet(0, 0));
}

std::unordered_set<int> IncrementalAlgo::sampleVertices(const std::unordered_set<int>& L_prev, double probability)
{
    std::unordered_set<int> S_i;
    return S_i;
}

bool IncrementalAlgo::kBoundedRulingSet(int u, int v)
{
    for (auto s : V)
    {
        Asmall[s] = Dsource(eps, INF, s, graph, INF);
        auto dists = Asmall[s].distance;

        for (int j = 0; j < dists.size(); j++)
        {
            if (dists[j] < r2)
            {
                H[s].insert({j, dists[j]});
            }
        }
    }

    B = FastMIS(H, {});
    return B.mis.size() <= k;
}

void IncrementalAlgo::insertEdge(int u, int v)
{
    graph[u].insert({v, 1});
    graph[v].insert({u, 1});

    for (auto s : V)
    {
        Asmall[s].addEdge(u, v, 1);
        auto dists = Asmall[s].distance;

        for (int j = 0; j < dists.size(); j++)
        {
            if (dists[j] < r2)
            {
                H[s].insert({j, dists[j]});
            }
        }
    }

    B = FastMIS(H, {});

    while (B.mis.size() > k)
    {
        r2 *= (1 + eps);
        for (auto s : V)
        {
            auto dists = Asmall[s].distance;

            for (int j = 0; j < dists.size(); j++)
            {
                if (dists[j] < r2)
                {
                    H[s].insert({j, dists[j]});
                }
            }
        }
        B = FastMIS(H, {});
    }
}

std::vector<int> IncrementalAlgo::getCenters()
{
    std::vector<int> res;

    for (auto mm : B.mis)
        res.push_back(mm);
    return res;
}
