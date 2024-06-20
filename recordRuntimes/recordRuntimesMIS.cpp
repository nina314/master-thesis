#include "../maximalndependentSet/FastMIS.hpp"
#include "../maximalndependentSet/KboundedMIS.hpp"
#include "../maximalndependentSet/GreedyMIS.hpp"
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

void initializeMISAlgorithms(const auto& adj, unordered_set<int>& VV, auto& fastMIS, auto& kMIS, auto& greedyMIS, long long& durationFastMIS, long long& durationKMIS, long long& durationGreedyMIS) {
    auto start = high_resolution_clock::now();
    fastMIS = FastMIS(adj, VV);
    auto stop = high_resolution_clock::now();
    durationFastMIS = duration_cast<microseconds>(stop - start).count();

    start = high_resolution_clock::now();
    kMIS = KboundedMIS(adj, INF);
    stop = high_resolution_clock::now();
    durationKMIS = duration_cast<microseconds>(stop - start).count();

    start = high_resolution_clock::now();
    greedyMIS = GreedyMIS(adj, INF);
    stop = high_resolution_clock::now();
    durationGreedyMIS = duration_cast<microseconds>(stop - start).count();
}

void updateAndLogMIS(auto& fastMIS, auto& kMIS, auto& greedyMIS, const auto& edgesToAdd, ofstream& runtimesInc, ofstream& costsInc) {
    int cnt = 0;
    for (const auto& [s, p] : edgesToAdd) {
        auto [d, w] = p;
        if (cnt++ > 300) break;

        auto start = high_resolution_clock::now();
        fastMIS.update(s, d);
        auto stop = high_resolution_clock::now();
        auto durationFastMIS = duration_cast<microseconds>(stop - start).count();

        start = high_resolution_clock::now();
        auto kRes = kMIS.update(s, d);
        stop = high_resolution_clock::now();
        auto durationKMIS = duration_cast<microseconds>(stop - start).count();

        start = high_resolution_clock::now();
        auto gRes = greedyMIS.update(s, d);
        stop = high_resolution_clock::now();
        auto durationGreedyMIS = duration_cast<microseconds>(stop - start).count();

        runtimesInc << durationFastMIS << " " << durationKMIS << " " << durationGreedyMIS << endl;
        costsInc << fastMIS.mis.size() << " " << kRes.size() << " " << gRes.size() << endl;
    }
}

int main(int argc, char *argv[]) {
    string part = argv[1];
    string name = argv[2];
    int k = INF;

    ofstream runtimesInc("results/" + part + "/IncrementalMIS/runtimes/" + name + "-incrementalMISRuntimes.txt");
    runtimesInc << "FastMIS KBoundedMIS GreedyMIS" << endl;

    ofstream costsInc("results/" + part + "/IncrementalMIS/costs/" + name + "-incrementalMISRuntimes.txt");
    costsInc << "FastMIS KBoundedMIS GreedyMIS" << endl;

    auto adj = getGraph("testingData/cleanedFiles/" + part + "/" + name + "-Edges.txt");
    auto edgesToAdd = getQueries("testingData/cleanedFiles/" + part + "/" + name + "-Queries.txt");

    unordered_set<int> VV; 
    for (int i = 0; i < adj.size(); i++) VV.insert(i);

    FastMIS fastMIS;
    KboundedMIS kMIS;
    GreedyMIS greedyMIS;
    long long durationFastMIS, durationKMIS, durationGreedyMIS;

    initializeMISAlgorithms(adj, VV, fastMIS, kMIS, greedyMIS, durationFastMIS, durationKMIS, durationGreedyMIS);
    runtimesInc << durationFastMIS << " " << durationKMIS << " " << durationGreedyMIS << endl;

    updateAndLogMIS(fastMIS, kMIS, greedyMIS, edgesToAdd, runtimesInc, costsInc);

    return 0;
}
