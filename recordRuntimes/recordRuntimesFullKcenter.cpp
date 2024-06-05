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

float randomFloat() {
    static default_random_engine e;
    static uniform_real_distribution<float> dist(0, 1);
    return dist(e);
}

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
    int k = stoi(argv[1]); // Number of centers to select
    int alpha = stoi(argv[2]); // Approximation factor
    double eps = stod(argv[3]);
    string part = argv[4];
    string name = argv[5]; 

    ofstream runtimes("results/" + part + "/FullKcenter/runtimes/" + name + "-fullKcenterRuntimes.txt");
    ofstream costs("results/" + part + "/FullKcenter/costs/" + name + "-fullKcenterCosts.txt");

    runtimes << "RIndependent Gonzalez BaselineGreedy Bottleneck FullyDynamic" << endl;
    costs << "RIndependent Gonzalez BaselineGreedy Bottleneck FullyDynamic" << endl;

    int maxWeight = 1;

    auto graph = getGraph("testingData/cleanedFiles/" + part + "/" + name + "-Edges.txt");
    auto edgesToAdd = getQueries("testingData/cleanedFiles/" + part + "/" + name + "-Queries.txt");

    auto fd = FullyDynamicAlgo(graph, k);
    int cnt = 0;
    
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
        

        if (t || randomFloat() >= 0.3) {
            graph[s].insert({d, w});
            graph[d].insert({s, w});
            
            auto start = high_resolution_clock::now();
            fd.addEdge(s, d, w);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            runtime6 = duration.count();

        } else {
            i--;
            auto pa = getRandomEdge(graph);
            if (pa.first == 0 && pa.second == 0) continue;
            d = pa.second;
            s = pa.first;
            
            if(graph[s].find({d, w})==graph[s].end()) continue;
            if(graph[d].find({s, w})==graph[d].end()) continue;
            
            auto start = high_resolution_clock::now();
            fd.deleteEdge(s, d);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            runtime6 = duration.count();
            
            graph[s].erase({d, w});
            graph[d].erase({s, w});
        
        }
        
        if (cnt++ > 400) break;
        
        auto start = high_resolution_clock::now();
        auto centers1 = distanceRIndependent(graph, k, maxWeight);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        auto runtime1 = duration.count();
        auto cost1 = cost(graph, centers1);

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
        auto runtime2 = duration.count();
        auto cost2 = cost(graph, centers2);

        start = high_resolution_clock::now();
        auto centers3 = baselineGreedy(graph, k, maxWeight);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime3 = duration.count();
        auto cost3 = cost(graph, centers3);

        start = high_resolution_clock::now();
        auto centers4 = bottleneck(graph, k, maxWeight);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime4 = duration.count();
        auto cost4 = cost(graph, centers4);

        costs << cost1 << " " << cost2 << " " << cost3 << " " << cost4 << " " << cost(graph, fd.getCenters()) << endl;
        runtimes << runtime1 << " " << runtime2 << " " << runtime3 << " " << runtime4 << " " << runtime6 << endl;
    }

    return 0;
}
