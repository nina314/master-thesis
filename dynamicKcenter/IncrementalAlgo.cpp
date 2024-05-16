#include "IncrementalAlgo.hpp"
#include <iostream>

using namespace std;

IncrementalAlgo::IncrementalAlgo(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int eps)
    : graph(graph), k(k), i(0), eps(eps) {
    n = graph.size();
    V.reserve(n); // Reserve space to prevent resizing
    for (int j = 0; j < n; j++)
        V.insert(j);

    r2 = 1;

    do
    {
        i = 0;
        r2 = (1 + eps) * r2;

        initialized = false;
        prevL = V;
        curL = V;
        
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
    if (curL.size() > 4 * k)
    {
        if (i == 0 || curL.size() <= prevL.size() / 2)
        {
            i++;
            prevL=curL;
            curL.clear();
            double gamma = prevL.size() / (2 * k) - 1;
            double probability = min(10.0 * log(graph.size()) / gamma, 1.0);

            auto si = sampleVertices(prevL, probability);

            S_i.insert(si.begin(), si.end());

            vector<int> centers;
            for (auto c : S_i)
                centers.push_back(c);

            auto connected_graph = buildGraphWithSources(graph, centers);
            if (i < Abig.size())
                Abig[i] = ScaledEStree(centers[0], connected_graph);

            auto dists = Abig[i].getDistances();
            for (int j = 0; j < graph.size(); j++)
            {
                if (j < dists.size() && dists[j] > r2)
                    curL.insert(j);
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
            S_union = curL;
            S_union.insert(S_i.begin(), S_i.end());
            
            for (auto s : S_union)
            {
                Asmall[s] = ScaledEStree(s, graph);
                auto dists = Asmall[s].getDistances();

                for (auto s2: S_union)
                {
                    if (s < dists.size() && s2 < dists.size() && dists[s2] < r2)
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
                auto dists = Asmall[s].getDistances();

                for (auto s2: S_union)
                {
                    if (s < dists.size() && s2 < dists.size() && dists[s2] < r2)
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
    // Ensure graph has enough capacity
    if (u >= graph.size() || v >= graph.size())
        return;

    graph[u].insert({v, w});
    graph[v].insert({u, w});

    if (i < Abig.size() && Abig[i].getDistances().size() > 0)
    {
        Abig[i].addEdge(u, v, w);
        auto dists = Abig[i].getDistances();
        
        for (auto it = curL.begin(); it != curL.end();)
        {
            auto l = *it;
            if (l < dists.size() && dists[l] < r2)
            {
                it = curL.erase(it);
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
