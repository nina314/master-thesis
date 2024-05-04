#include "baselineGreedy.hpp"
#include "../utils/common.hpp"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <algorithm>

using namespace std;

vector<int> baselineGreedy(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k, int maxWeight) {
    int n = graph.size();
    vector<int> independentSet;
    vector<int> distances(n, INF);

    int source = rand() % n;
    independentSet.push_back(source);

    while (independentSet.size() < k) {
        auto max_dist = max_element(distances.begin(), distances.end());
        int max_index = distance(distances.begin(), max_dist);
        
        independentSet.push_back(max_index);

        Dijkstra(graph, max_index, distances);
    }

    return independentSet;
}

