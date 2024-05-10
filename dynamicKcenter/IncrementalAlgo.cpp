#include "IncrementalAlgo.hpp"
#include <iostream>

using namespace std;

IncrementalAlgo::IncrementalAlgo(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int eps)
    : graph(graph), k(k), i(0), eps(eps) {
    n = graph.size();
    for (int j = 0; j < n; j++)
        V.insert(j);

    r2 = 1;

    do
    {
        i = 0;
        r2 = (1 + eps) * r2;

        initialized = false;
        L.clear();
        L.push_back(V);
        L.resize(n);

        S_i.clear();
        Abig.resize(n);
        Asmall.resize(n);
        H.resize(n);

    } while (!kBoundedRulingSet(0, 0));
}

unordered_set<int> IncrementalAlgo::sampleVertices(const unordered_set<int>& L_prev, double probability)
{
    unordered_set<int> Ss;
    for (int vertex : L_prev)
    {
        if ((double)rand() / RAND_MAX < probability)
        {
            Ss.insert(vertex);
        }
    }
    return Ss;
}

bool IncrementalAlgo::kBoundedRulingSet(int u, int v)
{
    if (L[i].size() > 4 * k)
    {
        if (i == 0 || L[i].size() <= L[i - 1].size() / 2)
        {
            i++;
            double gamma = L[i - 1].size() / (2 * k) - 1;
            double probability = min(10.0 * log(graph.size()) / gamma, 1.0);

            auto si = sampleVertices(L[i - 1], probability);

            S_i.insert(si.begin(), si.end());

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
            S_union.insert(S_i.begin(), S_i.end());
            
            for (auto s : S_union)
            {
                Asmall[s] = Dsource(eps, INF, s, graph, INF);
                auto dists = Asmall[s].distance;

                for (auto s2: S_union)
                {
                    if (dists[s2] < r2)
                    {
                        H[s].insert({s2, dists[s2]});
                        H[s2].insert({s, dists[s2]});
                    }
                }
            }

            B = FastMIS(H, S_union);
            initialized = true;

            return B.mis.size() < k;
        }
        else
        {
            for (auto s : S_union)
            {
                Asmall[s].addEdge(u, v, 1);
                auto dists = Asmall[s].distance;

                for (auto s2: S_union)
                {
                    if (dists[s2] < r2)
                    {
                        H[s].insert({s2, dists[s2]});
                        H[s2].insert({s, dists[s2]});
                        
                        if(s!=s2)
                            B.update(s, s2);
                    }
                }
            }
            return B.mis.size() < k;
        }
    }
    return B.mis.size() < k;
}

void IncrementalAlgo::insertEdge(int u, int v, int w)
{
    graph[u].insert({v, w});
    graph[v].insert({u, w});

    if (Abig[i].distance.size() > 0)
    {
        Abig[i].addEdge(u, v, w);

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

    kBoundedRulingSet(u, v);
}

vector<int> IncrementalAlgo::getCenters()
{
    vector<int> res;

    for (auto mm : B.mis)
        res.push_back(mm);
    return res;
}
