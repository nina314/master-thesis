#include "staticKcenter/gonzales.hpp"
#include "staticKcenter/distanceRIndependent.hpp"
#include "staticKcenter/randomCenters.hpp"
#include "staticKcenter/baselineGreedy.hpp"
#include "staticKcenter/bottleneck.hpp"
#include "dynamicKcenter/IncrementalAlgo.hpp"
#include "dynamicKcenter/ModifiedIncrementalAlgo.hpp"
#include "dynamicKcenter/FullyDynamicAlgo.hpp"
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
    string name = argv[4]; // //edit-enwikibooks, fb-messages, ia-contacts_hypertext2009, ia-digg-reply

    ofstream runtimes("results/" + name + "-2incrementalKcenterRuntimes.txt");
    ofstream costs("results/" + name + "-2incrementalKcenterCosts.txt");

    runtimes << "RIndependent Gonzalez BaselineGreedy Bottleneck Random Dynamic FullyDynamic ModifiedIncremental" << endl;
    costs << "RIndependent Gonzalez BaselineGreedy Bottleneck Random Dynamic FullyDynamic ModifiedIncremental" << endl;

    int maxWeight = 1;

    auto graph = getGraph("testingData/cleanedFiles/" + name + "-Edges.txt");
    auto edgesToAdd = getQueries("testingData/cleanedFiles/" + name + "-Queries.txt");

    auto ia = IncrementalAlgo(graph, k, eps);
    cout<<"DA";
    auto mia = ModifiedIncrementalAlgo(graph, k, eps);
    cout<<"DA";
    auto fd = FullyDynamicAlgo(graph, k);
    cout<<"DA";
    
    for (auto [s, p] : edgesToAdd) {
        auto [d, w] = p;
        
        if(graph.size()<=s || graph.size()<=d) continue;
        if(graph[s].count({d,w}) || graph[d].count({s,w})) continue;
        
        graph[s].insert({d, w});
        graph[d].insert({s, w});
        
        auto start = high_resolution_clock::now();
        auto centers1 = distanceRIndependent(graph, k, maxWeight);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        auto runtime1 = duration.count();
        auto cost1 = cost(graph, centers1);
        cout<<"1 "<<cost1<<endl;
        
        start = high_resolution_clock::now();
        auto centers2 = gonzalezAlpha(graph, k, alpha, maxWeight);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime2 = duration.count();
        auto cost2 = cost(graph, centers2);
        cout<<"2 "<<cost2<<endl;
        
        start = high_resolution_clock::now();
        auto centers3 = baselineGreedy(graph, k, maxWeight);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime3 = duration.count();
        auto cost3 = cost(graph, centers3);
        cout<<"3 "<<cost3<<endl;
        
        start = high_resolution_clock::now();
        auto centers4 = bottleneck(graph, k, maxWeight);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime4 = duration.count();
        auto cost4 = cost(graph, centers4);
        cout<<"4 "<<cost4<<endl;
        
        start = high_resolution_clock::now();
//        ia.insertEdge(s, d, w);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime5 = duration.count();
//        auto cost5 = cost(graph, ia.getCenters());
        auto cost5=0;
        cout<<"5 "<<cost5<<endl;
        
        start = high_resolution_clock::now();
        mia.insertEdge(s, d, w);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime7 = duration.count();
        auto cost7 = cost(graph, mia.getCenters());
        
        start = high_resolution_clock::now();
        fd.addEdge(s, d, w);
        stop = high_resolution_clock::now();
        duration = duration_cast<microseconds>(stop - start);
        auto runtime6 = duration.count();
        auto cost6 = cost(graph, fd.getCenters());
        cout<<"6 "<<cost6<<endl;

        costs << cost1 << " " << cost2 << " " << cost3 << " " << cost4 << " " << cost5<< " "<< cost6 << " "<<cost7<<endl;
        
        runtimes << runtime1 << " " << runtime2 << " " << runtime3 << " " << runtime4 << " " << runtime5<<" "<<runtime6<<" "<<runtime7 << endl;
        
    }
    return 0;
}
