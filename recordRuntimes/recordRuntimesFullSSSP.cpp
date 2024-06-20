#include "../staticKcenter/EStree.hpp"
#include "../dynamicKcenter/FastDijkstra.hpp"
#include "../dynamicKcenter/ScaledEStree.hpp"
#include "../utils/common.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <numeric>

using namespace std;
using namespace std::chrono;

pair<int, int> getRandomEdge(auto& graph) {
    if (graph.empty()) return {0, 0};
    int s = rand() % graph.size();
    if (graph[s].empty()) return {0, 0};
    auto it = graph[s].begin();
    advance(it, rand() % graph[s].size());
    auto [d, w] = *it;
    return {s, d};
}

int main(int argc, char *argv[]) {
    int source = stoi(argv[1]);
    int D = stoi(argv[2]);
    double eps = stod(argv[3]);
    string part = argv[4];
    string name = argv[5];
    int m = 10;

    ofstream avgRuntimes("results/" + part + "/FullSSSP/" + name + "-avgRuntimes.txt");
    ofstream avgCosts("results/" + part + "/FullSSSP/" + name + "-avgCosts.txt");

    avgRuntimes << "k RIndependent Gonzalez BaselineGreedy Bottleneck FullyDynamic" << endl;
    avgCosts << "k RIndependent Gonzalez BaselineGreedy Bottleneck FullyDynamic" << endl;

    auto graph = getGraph("testingData/cleanedFiles/" + part + "/" + name + "-Edges.txt");
    auto edgesToAdd = getQueries("testingData/cleanedFiles/" + part + "/" + name + "-Queries.txt");

    int cnt = 0;
    auto dists = vector<int>(graph.size(), INF);

    EStree es(graph, source);
    FastDijkstra fd(graph, source);
    ScaledEStree sES(source, graph);

    long long runtimeSum1 = 0, runtimeSum2 = 0, runtimeSum3 = 0, runtimeSum4 = 0, runtimeSum5 = 0;
    int iterations = 0;

    for (int i = 0; i < edgesToAdd.size(); i++) {
        auto [s, p] = edgesToAdd[i];
        auto [d, w] = p;

        if (graph.size() <= s || graph.size() <= d) continue;
        if (graph[s].count(d) || graph[d].count(s)) continue;

        bool t = false;
        dists = vector<int>(graph.size(), INF);
        Dijkstra(graph, source, dists);
        for (auto d : dists) {
            if (d > INF / 2) {
                t = true;
                break;
            }
        }

        if (t || rand() % 10 > 1) {
            graph[s][d] = w;
            graph[d][s] = w;

            auto start = high_resolution_clock::now();
            fd.addEdge(s, d, w);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);

            start = high_resolution_clock::now();
            es.deleteEdge(s, d);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum1 += duration.count();

            if (!t) {
                runtimeSum5 += duration.count();
            }
        } else {
            i--;
            auto pa = getRandomEdge(graph);
            if (pa.first == 0 && pa.second == 0) continue;
            d = pa.second;
            s = pa.first;

            if (graph[s].find(d) == graph[s].end() || graph[d].find(s) == graph[d].end()) continue;

            graph[s].erase(d);
            graph[d].erase(s);

            dists = vector<int>(graph.size(), INF);
            Dijkstra(graph, source, dists);
            for (auto d : dists) {
                if (d > INF / 2) {
                    t = true;
                    break;
                }
            }

            if (t) continue;

            auto start = high_resolution_clock::now();
            fd.deleteEdge(s, d);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            runtimeSum5 += duration.count();

            start = high_resolution_clock::now();
            es.deleteEdge(s, d);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum1 += duration.count();
        }

        if (t) continue;
        if (cnt++ > 300) break;
        iterations++;
    }

    avgRuntimes << (iterations != 0 ? runtimeSum1 / iterations : 0) << " "
                << (iterations != 0 ? runtimeSum2 / iterations : 0) << " "
                << (iterations != 0 ? runtimeSum5 / iterations : 0) << endl;

    return 0;
}
