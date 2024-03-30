#include "staticKcenter/gonzales.hpp"
#include "staticKcenter/distanceRIndependent.hpp"
#include "staticKcenter/randomCenters.hpp"
#include "staticKcenter/baselineGreedy.hpp"
#include "staticKcenter/bottleneck.hpp"
#include "dynamicKcenter/DecrementalAlgo.hpp"
#include "utils/common.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <queue>
#include <unordered_set>

int main(int argc, char *argv[]) {
    int k = stoi(argv[1]); // Number of centers to select
    int alpha = stoi(argv[2]); // Approximation factor
    double eps = stod(argv[3]);
    string name = argv[4];

    ofstream runtimes("results/" + name + "-staticKcenterRuntimes.txt");
    ofstream costs("results/" + name + "-staticKcenterCosts.txt");

    runtimes << "RIndependent Gonzalez BaselineGreedy Bottleneck Random Dynamic" << endl;
    costs << "RIndependent Gonzalez BaselineGreedy Bottleneck Random Dynamic" << endl;

    int maxWeight = 1;

    auto graph = getGraph("testingData/cleanedFiles/" + name + "-Edges.txt");
    auto edgesToRemove = getQueries("testingData/cleanedFiles/" + name + "-Queries.txt");

    auto da = DecrementalAlgo(graph, eps, k);
    da.initialize();

    for (auto [s, d] : edgesToRemove) {
        graph[s].erase({d, 1});
        graph[d].erase({s, 1});

        auto [runtime1, centers1] = getDuration(distanceRIndependent, graph, k, maxWeight);
        auto cost1 = cost(graph, centers1);

        auto [runtime2, centers2] = getDuration(gonzalezAlpha, graph, k, alpha, maxWeight);
        auto cost2 = cost(graph, centers2);

        auto [runtime3, centers3] = getDuration(baselineGreedy, graph, k, maxWeight);
        auto cost3 = cost(graph, centers3);

        auto [runtime4, centers4] = getDuration(bottleneck, graph, k, maxWeight);
        auto cost4 = cost(graph, centers4);

        auto [runtime5, centers5] = getDuration(randomCenters, graph, k);
        auto cost5 = cost(graph, centers5);

        auto [runtime6, centers6] = getDurationObject(&DecrementalAlgo::deleteEdge, da, s, d);
        auto cost6 = cost(graph, centers6);

        costs << cost1 << " " << cost2 << " " << cost3 << " " << cost4 << " " << cost5 << " " << cost6 << endl;
        runtimes << runtime1 << " " << runtime2 << " " << runtime3 << " " << runtime4 << " " << runtime5 << " " << runtime6 << endl;
    }
    return 0;
}
