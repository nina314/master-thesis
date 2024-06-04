#include "../utils/common.hpp"
#include <vector>
#include <unordered_set>
#include <queue>
#include <functional>
#include <iostream>

using namespace std;

class FastDijkstra
{
public:
    vector<int> dists;
    int n;
    int source;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> g;

    FastDijkstra() {}

    FastDijkstra(vector<unordered_set<pair<int, int>, PHash, PCompare>> g, int source2) : g(g)
    {
        source = source2;
        n = g.size();
        dists = vector<int>(n, INF);
        dists[source] = 0;
        updateFrom(source);
    }

    void addEdge(int u, int v, int w)
    {
        if (dists[u] > dists[v]) swap(u, v);

        g[u].insert({v, w});
        g[v].insert({u, w});
        updateFrom(u);
    }

private:
    void updateFrom(int start)
    {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({dists[start], start});
        vector<bool> visited(dists.size(), false);

        while (!pq.empty()) {
            auto [du, uId] = pq.top();
            pq.pop();
            if (visited[uId]) continue;
            visited[uId] = true;

            for (auto& e : g[uId]) {
                int v = e.first;
                int w = e.second;
                if (dists[v] > du + w) {
                    dists[v] = du + w;
                    pq.push({dists[v], v});
                }
            }
        }
    }
};