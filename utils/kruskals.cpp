#include "kruskals.hpp"
#include <unordered_set>
#include <iostream>

int find_parent(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return find_parent(parent, parent[i]);
}

void union_sets(int parent[], int rank[], int x, int y) {
    int x_root = find_parent(parent, x);
    int y_root = find_parent(parent, y);

    if (rank[x_root] < rank[y_root])
        parent[x_root] = y_root;
    else if (rank[x_root] > rank[y_root])
        parent[y_root] = x_root;
    else {
        parent[y_root] = x_root;
        rank[x_root]++;
    }
}

vector<unordered_set<pair<int, int>, PHash, PCompare>> kruskal_mst(vector<unordered_set<pair<int, int>, PHash, PCompare>> adj) {
    int V = adj.size();
    vector<unordered_set<pair<int, int>, PHash, PCompare>> mst(V);
    int e = 0;
    vector<pair<int, pair<int, int>>> edges;

    for (int u = 0; u < V; u++) {
        for (auto& p : adj[u]) {
            int v = p.first;
            int w = p.second;
            edges.push_back({w, {u, v}});
        }
    }

    sort(edges.begin(), edges.end());

    int parent[V], rank[V];
    for (int i = 0; i < V; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    int i = 0;
    while (e < V - 1 && i < edges.size()) {
        int u = edges[i].second.first;
        int v = edges[i].second.second;
        int w = edges[i].first;
        i++;
        int x = find_parent(parent, u);
        int y = find_parent(parent, v);
        if (x != y) {
            e++;
            mst[u].insert({v, w});
            mst[v].insert({u, w});
            union_sets(parent, rank, x, y);
        }
    }

    return mst;
}
