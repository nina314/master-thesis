#include "dynamicSSSP/EStree.hpp"
#include "dynamicSSSP/DecrementalDynamicSSSP.hpp"
#include "dynamicSSSP/IncrementalDynamicSSSP.hpp"
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
    string name = argv[1]; // sequential
    int source = stoi(argv[2]); // 0
    int D = stoi(argv[3]); // 15
    int eps = stoi(argv[4]); // 1
    int m = 10, k = 3;

    ofstream runtimesInc("results/" + name + "-incrementalDynamicSSSPRuntimes.txt");
    runtimesInc << "ESTree IncrementalDynamicSSSP Dijkstra Dsource" << endl;

    auto adj = getGraph("testingData/cleanedFiles/" + name + "-Edges.txt");
    auto edgesToRemove = getQueries("testingData/cleanedFiles/" + name + "-Queries.txt");

    for (auto [s, d] : edgesToRemove) {
        adj[s].erase({d, 1});
        adj[d].erase({s, 1});
    }
    
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
    
    runtimesInc << duration.count() << " " << duration1.count() << " " << duration2.count() << " "<<duration3.count()<< endl;

    for (auto [s, d] : edgesToRemove) {
        
        adj[s].insert({d, 1});
        adj[d].insert({s, 1});
        
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

        runtimesInc << duration.count() << " " << duration1.count() << " " << duration2.count() <<" "<<duration3.count()<< endl;
        
//        auto d1s = es.getDistances();
//        auto d2s = dynamic2.d[0].Ds[source].distance;
//        int es=0, dd=0;
//        
//        for(int i=0; i<dists.size(); i++) 
//        {
//            if(d1s[i] == dists[i]) es++;
//            if(d2s[i] == dists[i]) dd++;
//        }
//        
//        cout<<es<<" "<<dd<<" "<<dists.size()<<endl;
        
    }
}
