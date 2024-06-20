#include "../staticKcenter/gonzales.hpp"
#include "../staticKcenter/distanceRIndependent.hpp"
#include "../staticKcenter/randomCenters.hpp"
#include "../staticKcenter/baselineGreedy.hpp"
#include "../staticKcenter/sparsification.hpp"
#include "../staticKcenter/bottleneck.hpp"
#include "../dynamicKcenter/FullyDynamicAlgo.hpp"
#include "../dynamicKcenter/DecrementalAlgo.hpp"
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
    int k_value = stoi(argv[1]); // Number of centers to select
    int alpha = stoi(argv[2]); // Approximation factor
    double eps = stod(argv[3]);
    string part = argv[4];
    string name = argv[5]; 

    ofstream avgRuntimes("results/" + part + "/FullKcenter/" + name + "-avgRuntimes.txt");
    ofstream avgCosts("results/" + part + "/FullKcenter/" + name + "-avgCosts.txt");

    avgRuntimes << "k Sparsification RIndependent Gonzalez BaselineGreedy Bottleneck FullyDynamic" << endl;
    avgCosts << "k Sparsification RIndependent Gonzalez BaselineGreedy Bottleneck FullyDynamic" << endl;

    for (int k = 15; k <= 50; k += 3) {
        cout<<k<<endl;
        auto graph = getGraph("testingData/cleanedFiles/" + part + "/" + name + "-Edges.txt");
        auto edgesToAdd = getQueries("testingData/cleanedFiles/" + part + "/" + name + "-Queries.txt");

        int maxWeight = 1;

        auto fd = FullyDynamicAlgo(graph, k);
        int cnt = 0;

        long long runtimeSum1 = 0, runtimeSum2 = 0, runtimeSum3 = 0, runtimeSum4 = 0, runtimeSum5 = 0, runtimeSum7 = 0;
        int costSum1 = 0, costSum2 = 0, costSum3 = 0, costSum4 = 0, costSum5 = 0, costSum7 = 0;
        int iterations = 0;

        for (int i = 0; i < edgesToAdd.size(); i++) {
            auto [s, p] = edgesToAdd[i];
            auto [d, w] = p;

            if (graph.size() <= s || graph.size() <= d) continue;
            if (graph[s].count({d, w}) || graph[d].count({s, w})) continue;

            int runtime6, cost6;

            auto dists = vector<int>(graph.size(), INF);
            Dijkstra(graph, 2, dists);
            bool t = false;
            for(auto d: dists)
                if(d>INF/2)
                {
                    t=true;
                    break;
                }


            if (t || rand()%10 > 1) {
                graph[s].insert({d, w});
                graph[d].insert({s, w});

                auto start = high_resolution_clock::now();
                fd.addEdge(s, d, w);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                
                if(!t)
                {
                runtimeSum5 += duration.count();
                costSum5 += cost(graph, fd.getCenters());
                }

            } else {
                i--;
                auto pa = getRandomEdge(graph);
                if (pa.first == 0 && pa.second == 0) continue;
                d = pa.second;
                s = pa.first;

                if(graph[s].find({d, w})==graph[s].end()) continue;
                if(graph[d].find({s, w})==graph[d].end()) continue;

                graph[s].erase({d, w});
                graph[d].erase({s, w});
                
                dists = vector<int>(graph.size(), INF);
                Dijkstra(graph, 2, dists);
                for(auto d: dists)
                    if(d>INF/2)
                    {
                        t=true;
                        break;
                    }

                if(t) continue;
                auto start = high_resolution_clock::now();
                fd.deleteEdge(s, d);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start);
                runtimeSum5 += duration.count();
                costSum5 += cost(graph, fd.getCenters());


            }

            if(t) continue;
//            cout<<cnt<<endl;
            if (cnt++ > 300) break;

            auto start = high_resolution_clock::now();
//            cout<<"SAD";
            auto centers1 = distanceRIndependent(graph, k, maxWeight);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            runtimeSum1 += duration.count();
            costSum1 += cost(graph, centers1);
//            cout<<"dri\n";

            for(auto ed: graph)
            {
                for(auto [sss, ww]: ed)
                {
                    maxWeight = max(maxWeight, ww);
                }
            }

            start = high_resolution_clock::now();
            auto centers2 = gonzalezAlpha(graph, k, alpha, maxWeight);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum2 += duration.count();
//            auto temp =min(graph.size(), cost(graph, centers2));
            costSum2+=cost(graph, centers2);
//            cost<<costSum2<<endl;
//            cout<<"gonz\n";
            
            start = high_resolution_clock::now();
            auto centers3 = baselineGreedy(graph, k, maxWeight);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum3 += duration.count();
            costSum3 += cost(graph, centers3);
//            cout<<"baseline\n";
            
            start = high_resolution_clock::now();
            auto centers4 = bottleneck(graph, k, maxWeight);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum4 += duration.count();
            costSum4 += cost(graph, centers4);
            
            start = high_resolution_clock::now();
            auto centers7 = sparsification(graph, k, 0.3, 0.3);
            stop = high_resolution_clock::now();
            duration = duration_cast<microseconds>(stop - start);
            runtimeSum7 += duration.count();
            costSum7 += cost(graph, centers7);
            
            iterations++;
        }

        avgRuntimes << k << " "
                    << (iterations != 0 ? runtimeSum7 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum1 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum2 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum3 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum4 / iterations : 0) << " "
                    << (iterations != 0 ? runtimeSum5 / iterations : 0) << endl;

        avgCosts << k << " "
                 << (iterations != 0 ? costSum7 / iterations : 0) << " "
                 << (iterations != 0 ? costSum1 / iterations : 0) << " "
                 << (iterations != 0 ? costSum2 / iterations : 0) << " "
                 << (iterations != 0 ? costSum3 / iterations : 0) << " "
                 << (iterations != 0 ? costSum4 / iterations : 0) << " "
                 << (iterations != 0 ? costSum5 / iterations : 0) << endl;
    }

    return 0;
}
