#ifndef DSOURCE_HPP
#define DSOURCE_HPP

#include "../utils/common.hpp"
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Dsource {
private:
    unordered_map<int, unordered_map<int, int>> graph;
    int epsilon;
    int maxDegree;
    int source;
    int m;
    int mm;

public:
    unordered_map<int, int> parent;
    vector<int> distance, res;
    Dsource() {}
    Dsource(double eps, int degree, int src, vector<unordered_set<pair<int, int>, PHash, PCompare>> g, int mmax);
    pair<unordered_set<int>, unordered_set<int>> addEdge(int u, int v, int weight);
};

#endif // DSOURCE_HPP
