#include "../dynamicSSSP/FullyDynamicScaledEStree.hpp"
#include "../utils/common.hpp"
#include <unordered_set>
#include <vector>

class FullyDynamicAlgo
{
    public:
    FullyDynamicScaledEStree D;
    int s;
    unordered_set<int> centers;
    int k;
    
    FullyDynamicAlgo(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k2)
    {   
        k= k2;
        preprocessing(graph, k);
    }
    
    unordered_set<int> simulateGonzales()
    {
        unordered_set<int> C;
        auto tempD = D;
        
        for(int i=0; i<k; i++)
        {
            auto dists= D.getDistances();
            auto maxi = max_element(dists.begin(), dists.end());
            int c_next = distance(dists.begin(), maxi); 
            
            D.addEdge(s, c_next, 0);
            C.insert(c_next);
        }
        
        D = tempD;
//        for(auto c: C)
//        {
//            cout<<c<<endl;
//            D.deleteEdge(s, c);
//        }
        return C;
    }
    
    void preprocessing(vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int k)
    {
        s= graph.size();
        graph.push_back({});
        
        D = FullyDynamicScaledEStree(s, graph);
        centers = simulateGonzales();
    }
    
    void addEdge(int u, int v, int w)
    {
        D.addEdge(u, v, w);
        centers = simulateGonzales(); 
    }
    
    void deleteEdge(int u, int v)
    {
        D.deleteEdge(u, v);
        centers = simulateGonzales(); 
    }
    
    vector<int> getCenters()
    {
        vector<int> res;

        for (auto cen : centers)
            res.push_back(cen);
        
        return res;
    }

};

