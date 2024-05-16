#include "dynamicSSSP/EStree.hpp"
#include "dynamicSSSP/DecrementalDynamicSSSP.hpp"
#include "dynamicSSSP/IncrementalDynamicSSSP.hpp"
#include "dynamicSSSP/ScaledEStree.hpp"
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
    int m = 10, k = 3;

    ofstream runtimesInc("results/short/IncrementalSSSP/" + name + "-incrementalDynamicSSSPRuntimes.txt");
    runtimesInc << "EStree IncrementalDynamicSSSP Dijkstra Dsource ScaledEStree" << endl;

    auto adj = getGraph("testingData/cleanedFiles/short/" + name + "-Edges.txt");
    auto edgesToAdd = getQueries("testingData/cleanedFiles/short/" + name + "-Queries.txt");
    cout<<"testingData/cleanedFiles/short/" + name + "-Edges.txt"<<endl;

    auto start = high_resolution_clock::now();
    EStree es(adj, source);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    start = high_resolution_clock::now();
    IncrementalDynamicSSSP dynamic(k, eps, adj, m, D);
    stop = high_resolution_clock::now();
    auto duration1 = duration_cast<microseconds>(stop - start);
    
    vector<int> dists(adj.size(), INF);
    start = high_resolution_clock::now();
    Dijkstra(adj, source, dists);
    stop = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop - start);
    
    start = high_resolution_clock::now();
    Dsource dSource(eps, D, source, adj, D);
    stop = high_resolution_clock::now();
    auto duration3 = duration_cast<microseconds>(stop - start);
    
    start = high_resolution_clock::now();
    ScaledEStree scaledES(source, adj);
    stop = high_resolution_clock::now();
    auto duration4 = duration_cast<microseconds>(stop - start);
    
    runtimesInc << duration.count() << " " << duration1.count() << " " << duration2.count() << " "<<duration3.count()<<" "<< duration4.count()<<endl;

    for (auto [s, p] : edgesToAdd) {
        auto [d, w] = p;
        
        adj[s].insert({d, w});
        adj[d].insert({s, w});
        
        start = high_resolution_clock::now();
        es.addEdge(s, d);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        
        start = high_resolution_clock::now();
        dynamic.insert(s, d, 1);
        stop = high_resolution_clock::now();
        duration1 = duration_cast<microseconds>(stop - start);
        
        start = high_resolution_clock::now();
        dists = vector<int>(adj.size(), INF);
        Dijkstra(adj, source, dists);
        stop = high_resolution_clock::now();
        duration2 = duration_cast<microseconds>(stop - start);
        
        start = high_resolution_clock::now();
        dSource.addEdge(s, d, 1);
        stop = high_resolution_clock::now();
        duration3 = duration_cast<microseconds>(stop - start);
        start = high_resolution_clock::now();
        
        scaledES.addEdge(s, d, 1);
        stop = high_resolution_clock::now();
        duration4 = duration_cast<microseconds>(stop - start);
        
        runtimesInc << duration.count() << " " << duration1.count() << " " << duration2.count() <<" "<<duration3.count()<<" "<< duration4.count()<<endl;
    }
}
