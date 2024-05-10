#include "../utils/common.hpp"
#include "../dynamicSSSP/EStree.hpp"
#include <unordered_set>
#include <vector>

using namespace std;

class DecrementalAlgo
{
    public:
    EStree es;
    int r, r2, eps;
    int lo, hi;
    int k;
    vector<int> M;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> connectedGraph;
    vector<unordered_set<pair<int, int>, PHash, PCompare>> originalGraph;
    
    DecrementalAlgo(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int epss, int kk);
    vector<int> initialize();
    bool tryInitialize(int r, int r2);
    bool tryDeleteEdge(int s, int d);
    vector<int> deleteEdge(int s, int d);
    vector<int> getCenters();
};
