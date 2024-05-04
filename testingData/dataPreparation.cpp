#include "../utils/common.hpp"
#include "../utils/kruskals.hpp"
#include <vector>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <random>
#include <queue>

using namespace std;

void makeFiles(vector<unordered_set<pair<int, int>, PHash, PCompare>>& spanningTree, string name, int n)
{
    ofstream queries("testingData/cleanedFiles/"+name+"-Queries.txt");
    ofstream edges("testingData/cleanedFiles/"+name+"-Edges.txt");
    
    random_device rd;
    mt19937 gen(rd()); 

    uniform_int_distribution<> dist(0, spanningTree.size()-1); 

    for(int i=0; i<n; i++)
    {
        int random1 = dist(gen);
        int random2 = dist(gen);
        
        while(spanningTree[random1].find({random2, 1}) != spanningTree[random1].end() || random1==random2)
        {
            random1 = dist(gen);
            random2 = dist(gen);
        }
        spanningTree[random1].insert({random2, 1});
        spanningTree[random2].insert({random1, 1});
        queries<< random1<<" "<< random2<<endl;
    }
    
    for(int i=0; i<spanningTree.size(); i++)
    {
        for(auto [nei, wei]: spanningTree[i])
        {
            edges<< i<<" "<< nei<< " "<<wei<<endl;
            
        }
    }
}

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

int main()
{
//    auto spanningTree = makeSequential(50);
    
    auto adj = getGraph("testingData/originalFiles/email-Eu-core.txt", 10);
    makeConnected(adj);
    
    int n=0;
    auto spanningTree = kruskal_mst(adj);
    
    for(int i=0; i<spanningTree.size(); i++)
    {
        for(auto [nei, wei]: spanningTree[i])
        {
            n++;
        }
    }
       
    makeFiles(spanningTree, "email-Eu-core-shortened", 30);
}