#include "dynamicSSSP/EStree.hpp"
#include "dynamicSSSP/ScaledEStree.hpp"
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
#include <chrono>

using namespace chrono;

int main(int argc, char *argv[]) {
    string name = argv[1];
    int source = stoi(argv[2]);
    int D = stoi(argv[3]);
    int eps = stoi(argv[4]); 
    int m = 10;

    ofstream runtimesDec("results/short/DecrementalSSSP/" + name + "-decrementalDynamicSSSPRuntimes.txt");
    
    runtimesDec << "EStree DecrementalDynamicSSSP Dijkstra ScaledEStree" << endl;

    auto graph = getGraph("testingData/cleanedFiles/short/" + name + "-Edges.txt");
    auto edgesToAdd = getQueries("testingData/cleanedFiles/short/" + name + "-Queries.txt");

    for (auto [s, p] : edgesToAdd) {
        auto [d, w] = p;
        
        if(graph.size()<=s || graph.size()<=d) continue;
        
        graph[s].insert({d, w});
        graph[d].insert({s, w});
    }
    
    
    auto start = high_resolution_clock::now();
    EStree es(graph, source);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    DecrementalDynamicSSSP dynamic(graph, D, eps, source);
    stop = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop - start);
    
    vector<int> dists(graph.size(), INF);
    start = high_resolution_clock::now();
    Dijkstra(graph, source, dists);
    stop = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop - start);
    
    start = high_resolution_clock::now();
    ScaledEStree scaledES(source, graph);
    stop = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(stop - start);
    
    runtimesDec << duration.count() << " " << duration1.count() << " " << duration2.count() << " "<<duration3.count()<< endl;

    for (auto [s, p] : edgesToAdd) {
        auto [d, w] = p;
        
        if(graph.size()<=s || graph.size()<=d) continue;
        
        graph[s].erase({d, w});
        graph[d].erase({s, w});
        
        start = high_resolution_clock::now();
        es.deleteEdge(s, d, INF/2);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);

        
        start = high_resolution_clock::now();
        dynamic.deleteEdge(source, d);
        stop = high_resolution_clock::now();
        duration1 = duration_cast<microseconds>(stop - start);
        
        start = high_resolution_clock::now();
        dists = vector<int>(graph.size(), INF);
        Dijkstra(graph, source, dists);
        stop = high_resolution_clock::now();
        duration2 = duration_cast<microseconds>(stop - start);

        start = high_resolution_clock::now();
        scaledES.deleteEdge(s, d);
        stop = high_resolution_clock::now();
        duration3 = duration_cast<microseconds>(stop - start);

        runtimesDec << duration.count() << " " << duration1.count() << " " << duration2.count() <<" "<<duration3.count()<< endl;
    }
}
