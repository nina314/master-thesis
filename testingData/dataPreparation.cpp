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

ofstream edges; // Declare edges globally
vector<unordered_set<int>> spanning_tree;

void bfs(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int start, vector<bool>& visited) {
    vector<pair<int, int>> path;
    queue<pair<int, int>> q;
    q.push({start, 0});
    visited[start] = true;

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();
        
        if (path.size() > 0 && path[path.size() - 1]!=current)
        {
            edges << path[path.size() - 1].first << " " << current.first << " " << current.second << endl;
            spanning_tree[path[path.size() - 1].first].insert(current.first);
            spanning_tree[current.first].insert(path[path.size() - 1].first);
        }
        path.push_back(current);
        for (auto neighbor : graph[current.first]) {
            if (!visited[neighbor.first]) {
                visited[neighbor.first] = true;
                q.push(neighbor);
            }
        }
    }
}

void makeConnected(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph) {
    vector<bool> visited(graph.size(), false);

    for (int i = 0; i < graph.size(); i++) {
        if (visited[i]) continue;

        if (i != 0) {
            graph[i - 1].insert({i, 1});
            graph[i].insert({i - 1, 1});
            spanning_tree[i].insert(i-1);
            spanning_tree[i-1].insert(i);
            edges << i << " " << i - 1 << " " << 1 << endl;
        }

        bfs(graph, i, visited);
    }
}

vector<unordered_set<pair<int, int>, PHash, PCompare>> makeSequential(int nodes) {
    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph(nodes + 1);

    for (int i = 0; i < nodes; ++i) {
        graph[i].insert({i + 1, 1});
        graph[i + 1].insert({i, 1});
    }

    return graph;
}

void getWhole(string name, int maxDeg1, int maxDeg2) {
    ifstream original("testingData/originalFiles/short/" + name + ".txt");
    ofstream queries("testingData/cleanedFiles/short/" + name + "-Queries.txt");
    edges.open("testingData/cleanedFiles/short/" + name + "-Edges.txt");

    vector<unordered_set<pair<int, int>, PHash, PCompare>> adj;

    string a, b, c = "1";
    vector<pair<int, pair<int, int>>> ques;

    vector<int> tr;
    
    while (original >> a >> b >> c) {
        int a1 = stoi(a), b1 = stoi(b), c1 = stoi(c);

        if (adj.size() <= max(a1, b1)) {
            adj.resize(max(a1, b1) + 1);
            tr.resize(max(a1, b1) + 1);
        }
        
        if(tr[a1]>maxDeg1 && tr[b1]>maxDeg1) continue;
        tr[a1]++;
        tr[b1]++;
        
        if(a1==b1) continue;
        
        ques.push_back({a1, {b1, c1}});

        if(adj[a1].size()>maxDeg2 && adj[b1].size()>maxDeg2) continue;
        adj[a1].insert({b1, c1});
        adj[b1].insert({a1, c1});
    }
    
    spanning_tree.resize(adj.size());

    makeConnected(adj);
    
    for(auto [a, p]: ques)
    {
        auto [b, w] = p;
        if(!adj[a].count({b, w}))
            queries << a << " " << b << " " << w << endl;
    }
    
    edges.close();
}

void getWholePlain(string name) {
    ifstream original("testingData/originalFiles/short/" + name + ".txt");
    ofstream queries("testingData/cleanedFiles/short/" + name + "-Queries.txt");
    edges.open("testingData/cleanedFiles/short/" + name + "-Edges.txt", std::ios_base::app);


    vector<unordered_set<pair<int, int>, PHash, PCompare>> adj;

    string a, b, c = "1";
    vector<pair<int, pair<int, int>>> ques;

    while (original >> a >> b >> c) {
        int a1 = stoi(a), b1 = stoi(b), c1 = stoi(c);

        if (adj.size() <= max(a1, b1)) {
            adj.resize(max(a1, b1) + 1);
        }

        adj[a1].insert({b1, c1});
        adj[b1].insert({a1, c1});
    }

    for (auto it = adj.begin(); it != adj.end();) {
        if (it->empty()) {
            it = adj.erase(it);
        } else {
            ++it;
        }
    }

    for (int i = 0; i < adj.size(); i++) {
        edges << i << " " << adj[i].begin()->first << " " << adj[i].begin()->second << endl;
        for (auto [nei, wei] : adj[i]) {
            queries << a << " " << nei << " " << wei << endl;
        }
    }
    edges.close();
}

int main()
{
    string directory_path = "../orig_files/";
    vector<pair<int, int>> degs = {{10, 1}, {14, 1}, {18, 1}, {6, 1}};
    int i=0;
    
    for (const auto& entry : fs::directory_iterator(directory_path)) {
        
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            string filename = entry.path().stem().string();
            
            cout<<filename<<endl;
            getWhole(filename, degs[i].first, degs[i].second);
            i++;
        }
    }

}