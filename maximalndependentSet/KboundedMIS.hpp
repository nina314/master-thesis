#include "../utils/common.hpp"
#include "../utils/DynamicHeap.hpp"
#include <unordered_set>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class KboundedMIS {
private:
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    int k;
    vector<int> degrees;
    unordered_set<int> vertices;

public:
    unordered_set<int> mis;
    KboundedMIS() = default;
    KboundedMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k);
    unordered_set<int> update(int u, int v);
};
