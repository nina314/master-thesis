#ifndef FAST_MIS_HPP
#define FAST_MIS_HPP

#include "../utils/common.hpp"
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm> 
#include <cmath> 
#include <queue>

using namespace std;

struct CustomCompare {
    vector<int> pi; 
    CustomCompare(){};
    
    CustomCompare(const vector<int>& _pi) : pi(_pi) {} 

    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        return pi[a.first] < pi[b.first];
    }
        
    bool operator()(const int& a, const int& b) const {
        return pi[a] < pi[b];
    }
};

class FastMIS {
public:
    FastMIS() = default;
    FastMIS(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, unordered_set<int> nodes);
    set<int, CustomCompare> GreedyMIS(unordered_set<int> nodes);
    unordered_set<int> findInfluencedSet(int u, int v, int b);
    void update(int u, int v);
    void printAll();
    set<int, CustomCompare> mis;

private:
    vector<set<pair<int, int>, CustomCompare>> G;
    vector<set<int, CustomCompare>> V;
    vector<int> pi;
    set<int, CustomCompare> VV;
    vector<int> enumerated;
    
    vector<int> generateRandomPermutation(int n);
    void fixSubgraphs(const unordered_set<int>& S, const set<int, CustomCompare>& before);
    vector<set<pair<int, int>, CustomCompare>> convertToSet(const vector<unordered_set<pair<int, int>, PHash, PCompare>>& unorderedSets);
};

#endif // FAST_MIS_HPP
