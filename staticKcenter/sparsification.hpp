#include "../utils/common.hpp"
#include <iostream>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <queue>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

vector<int> sparsification(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k, double alpha, double beta) {
    int n = graph.size();
    auto k_prime = max(log(static_cast<double>(n)), static_cast<double>(k));
    
    unordered_set<int> U;
    vector<int> global_centers;
    
    for(int i = 0; i < n; i++) {
        U.insert(i);
    }
    
    int cur_dist = 0;
    
    while(U.size() > alpha * k_prime) {
        vector<bool> to_take(U.size(), false);
        
        for(int i = 0; i < floor(alpha * k_prime); i++) {
            int tt = rand() % U.size();
            to_take[tt] = true;
        }
        
        vector<int> centers;
        int cnt = 0;
        
        for(auto it = U.begin(); it != U.end(); it++) {
            if(to_take[cnt]) {
                centers.push_back(*it);
                global_centers.push_back(*it);
            }
            cnt++;
        }
        
        graph = buildGraphWithSources(graph, centers);
        vector<int> distance(graph.size(), INF);

        int start = centers[0];
        
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        distance[start] = 0;
        pq.push({0, start});
        vector<bool> visited(distance.size(), false);
        int num_vis = 0;
        int needed_size = beta * U.size();
        
        while(num_vis < needed_size || (!pq.empty() && pq.top().first<=cur_dist)) 
        {
            auto [du, uId] = pq.top();
            pq.pop();
            if(visited[uId]) continue;
            visited[uId] = true;
            
            cur_dist = max(cur_dist, du);
            if(U.find(uId)!=U.end())
            {
                num_vis++;
                U.erase(uId);
            }
            
            for (auto& e : graph[uId]) 
            {
                int v = e.first;
                int w = e.second;
                if (distance[v] > du + w) 
                {
                    distance[v] = du + w;
                    pq.push({distance[v], v});
                }
            }
        }        
    }
    
    vector<int> indices(global_centers.size());
    vector<int> res;
    iota(indices.begin(), indices.end(), 0);
    
    random_device rd;
    mt19937 g(rd());
    shuffle(indices.begin(), indices.end(), g);

    for (int i = 0; i < min((int)global_centers.size(), (int)k); ++i) 
    {
        res.push_back(global_centers[indices[i]]);
    }
    
    cout<<"GLC size: "<<global_centers.size()<<endl;
    return res;
}