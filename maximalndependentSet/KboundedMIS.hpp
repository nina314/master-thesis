#include "../utils/common.hpp"
#include <unordered_set>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class KboundedMIS {
private:
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph;
    int k;
    bool isIndependent(int u, int v);
    void updateMIS();

public:
    unordered_set<int> mis;
    KboundedMIS() = default;
    KboundedMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k);
    unordered_set<int> addEdge(int u, int v);
    unordered_set<int> getMIS() { return mis; }
};
