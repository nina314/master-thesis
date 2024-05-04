#ifndef COMMON
#define COMMON

#include <vector>
#include <unordered_set>
#include <utility>
#include <chrono>
#include <functional>
#include <fstream>

using namespace std;
using namespace chrono;
const int INF = numeric_limits<int>::max();

struct PHash {
    size_t operator()(const pair<int, int>& p) const {
        return hash<int>{}(p.first);
    }    
};

struct PCompare {
    bool operator () (const pair<int, int>& lhs, const pair<int, int>& rhs) const {
        return lhs.first==rhs.first;
    }
};

void Dijkstra(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int start, vector<int>& distances);
vector<unordered_set<pair<int, int>, PHash, PCompare>> buildGraphWithSources(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, vector<int> centers);
int cost(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, vector<int> centers);
vector<unordered_set<pair<int, int>, PHash, PCompare>> getGraph(string name, int nodes = INF);
vector<pair<int, int>> getQueries(string name);

template<typename Func, typename... Args>
pair<double, decltype(declval<Func>()(declval<Args>()...))> getDuration(Func func, Args&&... args) 
{
    auto start = high_resolution_clock::now();
    auto result = func(forward<Args>(args)...);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    return { duration.count(), result };
}

template<typename Func, typename... Args>
double getDurationVoid(Func func, Args&&... args) 
{
    auto start = high_resolution_clock::now();
    func(forward<Args>(args)...);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    return duration.count();
}

template<typename Func, typename Object, typename... Args>
double getDurationObjectVoid(Func func, Object obj, Args&&... args) 
{
    auto start = high_resolution_clock::now();
    (obj.*func)(std::forward<Args>(args)...);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start); 
    return duration.count();
}

template<typename Func, typename Object, typename... Args>
pair<double, vector<int>> getDurationObject(Func func, Object obj, Args&&... args)  // decltype(declval<Func>()(declval<Args>()...)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto result = (obj.*func)(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start); 
    return { duration.count(), result };
}

#endif