#include "../utils/common.hpp"
#include <vector>
#include <unordered_set>

using namespace std;

vector<int> baselineGreedy(vector<unordered_set<pair<int, int>, PHash>> graph, int k, int maxWeight);
