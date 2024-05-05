#ifndef D_HPP
#define D_HPP

#include "Dsource.hpp"
#include <unordered_map>

using namespace std;

class D {
private:
    unordered_map<int, unordered_set<int>> U;
    double epsilon;
    int mm;
    int maxDegree;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;

public:
    unordered_map<int, Dsource> Ds;
    D();
    D(double eps, int degree, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int mm);
    void insertVertex(int u);
    unordered_map<int, unordered_map<int, int>> addEdge(int v1, int v2, int weight);
    int getDistance(int v1, int v2);
};

#endif
