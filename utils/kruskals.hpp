#ifndef KRUSKAL
#define KRUSKAL

#include "common.hpp"
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

int find_parent(int parent[], int i);
void union_sets(int parent[], int rank[], int x, int y);
vector<unordered_set<pair<int, int>, PHash, PCompare>> kruskal_mst(vector<unordered_set<pair<int, int>, PHash, PCompare>> adj);

#endif
