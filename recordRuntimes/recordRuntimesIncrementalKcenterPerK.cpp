#include "../staticKcenter/gonzales.hpp"
#include "../staticKcenter/distanceRIndependent.hpp"
#include "../staticKcenter/randomCenters.hpp"
#include "../staticKcenter/baselineGreedy.hpp"
#include "../staticKcenter/bottleneck.hpp"
#include "../dynamicKcenter/IncrementalAlgo.hpp"
#include "../dynamicKcenter/ModifiedIncrementalAlgo.hpp"
#include "../dynamicKcenter/FullyDynamicAlgo.hpp"
#include "../utils/common.hpp"
#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <queue>
#include <unordered_set>

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {
    int k = stoi(argv[1]); // Number of centers to select
    int alpha = stoi(argv[2]); // Approximation factor
    double eps = stod(argv[3]);
    string part = argv[4];
    string name = argv[5]; 
    
    ofstream avgRuntimes("results/"+part+"/IncrementalKcenter/"+ name + "-avgRuntimes.txt");
    ofstream avgCosts("results/"+part+"/IncrementalKcenter/"+ name + "-avgCosts.txt");

    avgRuntimes << "k RIndependent Gonzalez BaselineGreedy Bottleneck Incremental FullyDynamic" << endl;
    avgCosts << "k RIndependent Gonzalez BaselineGreedy Bottleneck Incremental FullyDynamic" << endl;

    for (k = 3; k <= 30; k += 3) {
        cout<<k<<endl;
        auto graph = getGraph("testingData/cleanedFiles/"+part+"/" + name + "-Edges.txt");
        auto edgesToAdd = getQueries("testingData/cleanedFiles/"+part+"/" + name + "-Queries.txt");

        int maxWeight = 1;

        for(auto edge: graph)
        {
            for(auto [s, w]: edge)
            {
                maxWeight = max(maxWeight, w);
            }
        }

        auto ia = IncrementalAlgo(graph, k, eps);
        auto fd = FullyDynamicAlgo(graph, k);
        int cnt = 0;

        long long runtimeSum1 = 0, runtimeSum2 = 0, runtimeSum3 = 0, runtimeSum4 = 0, runtimeSum5 = 0, runtimeSum6 = 0;
        int costSum1 = 0, costSum2 = 0, costSum3 = 0, costSum4 = 0, costSum5 = 0, costSum6 = 0;
        int iterations = 0;

        for (auto [s, p] : edgesToAdd) {
            auto [d, w] = p;

            if(graph.size()<=s || graph.size()<=d) continue;
            if(graph[s].count({d,w}) || graph[d].count({s,w})) continue;

            cout<<cnt<<endl;
            if (cnt++ > 300) break;

            graph[s].insert({d, w});
            graph[d].insert({s, w});
            maxWeight = max(maxWeight, w);

            auto start = high_resolution_clock::now();
            auto centers1 = distanceRIndependent(graph, k, maxWeight);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            runtimeSum1 += duration.count();
            costSum1 += cost(graph, centers1);

            start = high_resolution_clock::now();
            auto centers2 = gonzalezAlpha(graph, k, alpha, maxWeight);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum2 += duration.count();
            costSum2 += cost(graph, centers2);

            start = high_resolution_clock::now();
            auto centers3 = baselineGreedy(graph, k, maxWeight);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum3 += duration.count();
            costSum3 += cost(graph, centers3);

            start = high_resolution_clock::now();
            auto centers4 = bottleneck(graph, k, maxWeight);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum4 += duration.count();
            costSum4 += cost(graph, centers4);

            start = high_resolution_clock::now();
            ia.insertEdge(s, d, w);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum5 += duration.count();
            costSum5 += cost(graph, ia.getCenters());

            start = high_resolution_clock::now();
            fd.addEdge(s, d, w);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum6 += duration.count();
            costSum6 += cost(graph, fd.getCenters());

            iterations++;
        }

        avgRuntimes << k << " "
                    << (iterations != 0 ? runtimeSum1 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum2 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum3 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum4 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum5 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum6 / iterations : 0) << endl;

        avgCosts << k << " "
                 << (iterations != 0 ? costSum1 / iterations : 0) << " "
                 << (iterations != 0 ? costSum2 / iterations : 0) << " "
                 << (iterations != 0 ? costSum3 / iterations : 0) << " "
                 << (iterations != 0 ? costSum4 / iterations : 0) << " "
                 << (iterations != 0 ? costSum5 / iterations : 0) << " "
                 << (iterations != 0 ? costSum6 / iterations : 0) << endl;
    }

    return 0;
}
