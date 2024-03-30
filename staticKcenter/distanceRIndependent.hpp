#include "../utils/common.hpp"
#include <vector>
#include <unordered_set>

using namespace std;

vector<int> maximalDistanceRIndependentSet(vector<unordered_set<pair<int, int>, PHash>>& graph, int r);
vector<int> distanceRIndependent(vector<unordered_set<pair<int, int>, PHash>>& graph, int k, int maxWeight);
