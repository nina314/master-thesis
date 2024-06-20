#include "../dynamicSSSP/EStree.hpp"
#include "../dynamicSSSP/DecrementalDynamicSSSP.hpp"
#include "../dynamicSSSP/IncrementalDynamicSSSP.hpp"
#include "../dynamicSSSP/ScaledEStree.hpp"
#include "../utils/common.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <random>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <chrono>

using namespace chrono;

void initializeSSSPAlgorithms(const auto& adj, int source, int k, int eps, int m, int D, long long& durationES, long long& durationIncDynamic, long long& durationDijkstra, long long& durationDsource, long long& durationScaledES, auto& es, auto& dynamic, auto& dists, auto& dSource, auto& scaledES) {
    auto start = high_resolution_clock::now();
    es = EStree(adj, source);
    auto stop = high_resolution_clock::now();
    durationES = duration_cast<microseconds>(stop - start).count();

    start = high_resolution_clock::now();
    dynamic = IncrementalDynamicSSSP(k, eps, adj, m, D);
    stop = high_resolution_clock::now();
    durationIncDynamic = duration_cast<microseconds>(stop - start).count();

    dists = vector<int>(adj.size(), INF);
    start = high_resolution_clock::now();
    Dijkstra(adj, source, dists);
    stop = high_resolution_clock::now();
    durationDijkstra = duration_cast<microseconds>(stop - start).count();

    start = high_resolution_clock::now();
    dSource = Dsource(eps, D, source, adj, D);
    stop = high_resolution_clock::now();
    durationDsource = duration_cast<microseconds>(stop - start).count();

    start = high_resolution_clock::now();
    scaledES = ScaledEStree(source, adj);
    stop = high_resolution_clock::now();
    durationScaledES = duration_cast<microseconds>(stop - start).count();
}

void updateAndLogSSSP(auto& adj, auto& edgesToAdd, int source, ofstream& runtimesInc, auto& es, auto& dynamic, auto& dists, auto& dSource, auto& scaledES) {
    int cnt = 0;
    for (const auto& [s, p] : edgesToAdd) {
        auto [d, w] = p;
        if (cnt++ > 400) break;

        adj[s].insert({d, w});
        adj[d].insert({s, w});

        auto start = high_resolution_clock::now();
        es.addEdge(s, d);
        auto stop = high_resolution_clock::now();
        auto durationES = duration_cast<microseconds>(stop - start).count();

        start = high_resolution_clock::now();
        dynamic.insert(s, d, 1);
        stop = high_resolution_clock::now();
        auto durationIncDynamic = duration_cast<microseconds>(stop - start).count();

        dists = vector<int>(adj.size(), INF);
        start = high_resolution_clock::now();
        Dijkstra(adj, source, dists);
        stop = high_resolution_clock::now();
        auto durationDijkstra = duration_cast<microseconds>(stop - start).count();

        start = high_resolution_clock::now();
        dSource.addEdge(s, d, 1);
        stop = high_resolution_clock::now();
        auto durationDsource = duration_cast<microseconds>(stop - start).count();

        start = high_resolution_clock::now();
        scaledES.addEdge(s, d, 1);
        stop = high_resolution_clock::now();
        auto durationScaledES = duration_cast<microseconds>(stop - start).count();

        runtimesInc << durationES << " " << durationIncDynamic << " " << durationDijkstra << " " << durationDsource << " " << durationScaledES << endl;
    }
}

int main(int argc, char *argv[]) {
    int source = stoi(argv[1]);
    int D = stoi(argv[2]);
    int eps = stoi(argv[3]);
    string part = argv[4];
    string name = argv[5];
    int m = 10, k = 3;

    ofstream runtimesInc("results/" + part + "/IncrementalSSSP/" + name + "-incrementalDynamicSSSPRuntimes.txt");
    runtimesInc << "EStree IncrementalDynamicSSSP Dijkstra Dsource ScaledEStree" << endl;

    auto adj = getGraph("testingData/cleanedFiles/" + part + "/" + name + "-Edges.txt");
    auto edgesToAdd = getQueries("testingData/cleanedFiles/" + part + "/" + name + "-Queries.txt");

    EStree es;
    IncrementalDynamicSSSP dynamic;
    vector<int> dists;
    Dsource dSource;
    ScaledEStree scaledES;
    long long durationES, durationIncDynamic, durationDijkstra, durationDsource, durationScaledES;

    initializeSSSPAlgorithms(adj, source, k, eps, m, D, durationES, durationIncDynamic, durationDijkstra, durationDsource, durationScaledES, es, dynamic, dists, dSource, scaledES);
    runtimesInc << durationES << " " << durationIncDynamic << " " << durationDijkstra << " " << durationDsource << " " << durationScaledES << endl;

    updateAndLogSSSP(adj, edgesToAdd, source, runtimesInc, es, dynamic, dists, dSource, scaledES);

    return 0;
}
