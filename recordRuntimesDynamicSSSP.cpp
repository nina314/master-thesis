#include "dynamicSSSP/EStree.hpp"
#include "dynamicSSSP/DecrementalDynamicSSSP.hpp"
#include "utils/common.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <vector>

int main(int argc, char *argv[]) {
    string name = argv[1];
    int source = stoi(argv[2]);
    int D = stoi(argv[3]);
    int eps = stoi(argv[4]);

    ofstream runtimes("results/" + name + "-decrementalDynamicSSSPRuntimes.txt");
    runtimes << "ESTree Dijkstra DecrementalDynamicSSSP" << endl;

    auto adj = getGraph("testingData/cleanedFiles/" + name + "-Edges.txt");
    auto edgesToRemove = getQueries("testingData/cleanedFiles/" + name + "-Queries.txt");

    auto [duration, es] = getDuration([&adj, source]() -> EStree {
        auto es = EStree(adj, source);
        return es;
    });

    auto [duration2, dynamic] = getDuration([&adj, D, source, eps]() -> DecrementalDynamicSSSP {
        auto dy = DecrementalDynamicSSSP(adj, D, eps, source);
        return dy;
    });

    vector<int> dists(adj.size(), INF);
    auto duration1 = getDurationVoid(Dijkstra, adj, source, dists);

    runtimes << duration << " " << duration1 << " " << duration2 << endl;

    for (auto [s, d] : edgesToRemove) {
        adj[s].erase({d, 1});
        adj[d].erase({s, 1});

        dists = vector<int>(adj.size(), INF);
        duration1 = getDurationVoid(Dijkstra, adj, source, dists);
        duration = getDurationObjectVoid(&EStree::deleteEdge, es, s, d, INF / 2);
        duration2 = getDurationObjectVoid(&DecrementalDynamicSSSP::deleteEdge, dynamic, s, d);

        runtimes << duration << " " << duration1 << " " << duration2 << endl;
    }
}
