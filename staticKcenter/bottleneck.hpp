#include "../utils/common.hpp"
#include <vector>
#include<unordered_set>

using namespace std;

vector<int> bottleneckFixed(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int r);
vector<int> bottleneck(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int maxWeight);
