#include "../utils/common.hpp"
#include <vector>
#include <unordered_set>

using namespace std;

vector<int> maximalDistanceRIndependentSet(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int r, int k);
vector<int> distanceRIndependent(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int maxWeight);
