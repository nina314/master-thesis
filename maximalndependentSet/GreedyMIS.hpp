#include "../utils/common.hpp"
#include "../utils/DynamicHeap.hpp"
#include <unordered_set>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class GreedyMIS {
private:
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    int k;
    unordered_set<int> VV;

public:
    unordered_set<int> mis;
    GreedyMIS() = default;
    GreedyMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k);
    unordered_set<int> update(int u, int v);
};
