#include "../utils/common.hpp"
#include "../utils/kruskals.hpp"
#include <vector>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <random>
#include <queue>
#include <filesystem>


using namespace std;
namespace fs = filesystem;

void bfs(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int start, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (auto neighbor : graph[current]) {
            if (!visited[neighbor.first]) {
                visited[neighbor.first] = true;
                q.push(neighbor.first);
            }
        }
    }
}

void makeConnected(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph)
{
    vector<bool> visited(graph.size(), false);
    
    for(int i=0; i<graph.size(); i++)
    {
        if(visited[i]) continue;
        
        if(i!=0)
        {
            graph[i-1].insert({i, 1});
            graph[i].insert({i-1, 1});
        }
        
        bfs(graph, i, visited);
    }
}

vector<unordered_set<pair<int, int>, PHash, PCompare>> makeSequential(int nodes)
{
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph(nodes+1);
    
     for (int i = 0; i <nodes ; ++i) {
        graph[i].insert({i + 1, 1}); 
        graph[i + 1].insert({i, 1});
    }
    
    return graph;
}

void getWhole(string name)
{
    ifstream original("testingData/originalFiles/" + name + ".txt");
    ofstream queries("testingData/cleanedFiles/" + name + "-Queries.txt");
    ofstream edges("testingData/cleanedFiles/" + name + "-Edges.txt");

    vector<unordered_set<pair<int, int>, PHash, PCompare>> adj;

    string a, b, c = "1";

    while (original >> a >> b)
    {
        int a1 = stoi(a), b1 = stoi(b), c1 = stoi(c);

        if (adj.size() <= max(a1, b1))
        {
            adj.resize(max(a1, b1) + 1);
        }

        queries << a1 << " " << b1 << " " << c1 << endl;
        adj[a1].insert({b1, c1});
        adj[b1].insert({a1, c1});
    }
    
    for (auto it = adj.begin(); it != adj.end();)
    {
        if (it->empty())
        {
            it = adj.erase(it);
        }
        else
        {
            ++it;
        }
    }

    makeConnected(adj);
    auto spanningTree = kruskal_mst(adj);
    
     for (int i = 0; i < spanningTree.size(); i++)
     {
         for (auto [nei, wei] : spanningTree[i])
         {
             edges << i << " " << nei << " " << wei << endl;
         }
     }
}

int main()
{
    string directory_path = "testingData/originalFiles/";

    for (const auto& entry : fs::directory_iterator(directory_path)) {
        
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            string filename = entry.path().stem().string();
            getWhole(filename); //edit-enwikibooks
        }
    }

}