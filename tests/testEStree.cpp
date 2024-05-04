#include "../utils/common.hpp"
#include "../dynamicSSSP/EStree.hpp"
#include <iostream>

using namespace std;

int main()
{
    auto adj = getGraph("testingData/cleanedFiles/simpleGraph.txt");
    int source = 0;
    
    cout<<"start \n";
    EStree es(adj, source);
    auto dists = es.getDistances();
    
    for(auto d: dists) cout<<d<<" ";
    cout<<endl;
    
    es.addEdge(0, 2);
    dists = es.getDistances();
    for(auto d: dists) cout<<d<<" ";
    cout<<endl;
    
    es.addEdge(0, 3);
    dists = es.getDistances();
    for(auto d: dists) cout<<d<<" ";
    cout<<endl;
}