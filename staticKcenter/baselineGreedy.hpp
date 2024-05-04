#include "../utils/common.hpp"
#include <vector>
#include <unordered_set>

using namespace std;

vector<int> baselineGreedy(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k, int maxWeight);
