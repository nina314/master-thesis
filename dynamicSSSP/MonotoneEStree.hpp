#ifndef MT
#define MT

#include "../utils/common.hpp"
#include "../utils/DynamicHeap.hpp"
#include "EStree.hpp"
#include <queue>
#include <vector>
#include <unordered_set>

class MonotoneEStree
{
    int n, p, D, L;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> H;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;
    vector<DynamicHeap> N;
    vector<vector<int>> to_update;
    unordered_set<int> Qfind;
    vector<int> l;
    int src;
    
    public:
    MonotoneEStree();
    MonotoneEStree(int l, int src, vector<unordered_set<pair<int, int>, PHash, PCompare>> H);
    void updateH();
    void initialize();
    void deleteEdge(int u, int v);
    void increase(int u, int v, int w);
    void updateLevels(); 
    vector<int> getDistances();
};


#endif   
