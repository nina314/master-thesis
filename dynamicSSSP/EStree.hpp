#ifndef ES
#define ES

#include "../utils/common.hpp"
#include <tuple>
#include <vector>
#include <unordered_set>

using namespace std;

class EStree
{
    public:
    vector<unordered_set<int>> alpha;
    vector<unordered_set<int>> beta;
    vector<unordered_set<int>> gamma;
    vector<int> distances;
    int source;
    
    EStree();
    EStree(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int src, int maxDepth = INF);
    void deleteEdge(int s, int d, int maxDepth = INF);
    void addEdge(int s, int d) ;
    vector<int> getDistances();
};

#endif