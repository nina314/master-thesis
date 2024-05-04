#include "../utils/common.hpp"
#include <vector>
#include <set>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <utility>
#include <unordered_map>
#include <unordered_set>

using namespace std;

vector<int> gonzalezAlpha(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k, int alpha, int maxWeight);
