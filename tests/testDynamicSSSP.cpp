#include <gtest/gtest.h>
#include "../utils/common.hpp"
#include "../DecrementalDynamicSSSP/DecrementalDynamicSSSP.hpp"
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <vector>


TEST(DecrementalDynamicSSSP, SimpleGraph) {
    auto adj = getGraph("testingData/cleanedFiles/simpleGraph.txt");
 
    int n = adj.size();
    int p = ceil(log(n))/2;
    int D = ceil(sqrt(n))*pow(n, 1/p);
    
    DecrementalDynamicSSSP Ds(adj, 15, 0, 0);

    auto dists = Ds.getDistances();
        
    vector<int> expected_dists{0, 1, 2, 3, 2, 1};
    EXPECT_EQ(dists, expected_dists);
    
    Ds.deleteEdge(0, 1);
    dists = Ds.getDistances();
        
    expected_dists = vector<int>{0, 3, 2, 3, 2, 1};
    EXPECT_EQ(dists, expected_dists);
    
    Ds.deleteEdge(5, 4);
    dists = Ds.getDistances();
        
    expected_dists = vector<int>{0, 3, 2, 3, 2, 4};
    EXPECT_EQ(dists, expected_dists);
}

TEST(DecrementalDynamicSSSP, AmazonGraph) {
    auto adj = getGraph("testingData/cleanedFiles/email-Eu-core-Edges.txt");
    auto edgesToRemove = getQueries("testingData/email-Eu-core-Queries.txt");
    int source = 2, D = 15, eps = 0;
    
    EStree es(adj, source);
    DecrementalDynamicSSSP dynamic(adj, D, eps, source);
    
    for(auto [s, d]: edgesToRemove)
    {
        adj[s].erase({d, 1});
        adj[d].erase({s, 1});
        
        vector<int> dists(adj.size(), INF);
        
        Dijkstra(adj, source, dists);
        es.deleteEdge(s, d, INF/2);
        dynamic.deleteEdge(s, d);
        
        EXPECT_EQ(es.getDistances(), dists);
        EXPECT_EQ(dynamic.getDistances(), dists);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

