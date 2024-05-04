#include "IncrementalDynamicSSSP.hpp"
#include "../utils/common.hpp"

using namespace std;

IncrementalDynamicSSSP::IncrementalDynamicSSSP(int k, double eps, vector<unordered_set<pair<int, int>, PHash, PCompare>> graph, int m, int dMax)
    : epsilon(eps), k(k), m(m), dMax(dMax) {
    A.resize(k);
    U.resize(k);
    d.resize(k);
    W.resize(k);
    n.resize(k);
    r.resize(k);
        
    for (int i=0; i< graph.size(); i++) {
        A[0].insert(i);
    }

    auto [dd, mm] = getDepthForI(0);
    d[0] = D(eps, dd, graph, mm);

    for (int i = 1; i < k; ++i) {
        A[i].clear();
        auto [d_val, m_val] = getDepthForI(i);
        d[i] = D(eps, d_val, graph, m_val);
    }
}

pair<int, int> IncrementalDynamicSSSP::getDepthForI(int i) {
    auto mi = 2 * pow(m, (i + 1) / k);
    int power = ceil(log(pow(3 + 2 * epsilon, i) * dMax) / log(1 + epsilon));
    auto di = pow(1 + epsilon, power);
    return make_pair(di, mi);
}

void IncrementalDynamicSSSP::insert(int v1, int v2, int weight) {
    for (int i = 0; i < k; ++i) {
        U[i].clear();
    }

    for (int i = 0; i < k; ++i)
    {
        A[i].insert(U[i].begin(), U[i].end());

        auto T = d[i].addEdge(v1, v2, weight);
        cout<<"T "<<T.size()<<endl;
        unordered_set<int> setT;

        for (auto& [u, tree] : T) setT.insert(u);
        
        auto [dd, mm] = getDepthForI(i);
        int j = 1 + log(dd) / log(1 + epsilon);
        int pot = -1;

        auto nonEmpty = false;
        for (auto& [u, tree] : T) {
            for (auto [k, p] : tree) {
                if (W[i].count(k)) {
                    pot = k;
                    nonEmpty = true;
                }
            }

            if (nonEmpty) {
                n[i][u] = pot;
            } else {
                W[i].insert(setT.begin(), setT.end());
                for (auto v : setT)
                {
                    r[i][v] = u;
                }
                U[i+1].insert(u);
            }
        }
    
//        cout<<"A[]"<<i<<endl;
//        for(auto a: A[i]) cout<<a<<" ";
//        cout<<endl;
    }
    
    
}

int IncrementalDynamicSSSP::query(int u, int v) {
    int u0 = u;
    int s0 = 0;

    for (int i = 0; i < k; ++i) {
                
//        cout<<"D[]"<<i<<endl;
//        for(auto d: d[i].Ds[u0].distance) cout<<d<<" ";
        cout<<endl;
        if (d[i].Ds[u0].distance[v]!=numeric_limits<int>::max()/3) {
            return s0 + d[i].Ds[u0].distance[v];
        }
        if (!A[i + 1].count(u0)) {
            auto [dd, mm] = getDepthForI(i);
            int j = 1 + log(dd) / log(1 + epsilon);

            auto w = n[i][u0];
            s0 = s0 + d[i].Ds[u0].distance[w] + d[i].Ds[r[i][w]].distance[w];
            u0 = r[i][w];
        }
    }
    return s0;
}

//int main() {
//
////    unordered_map<int, unordered_map<int, int>> graph = {
////        {0, {{1, 10}, {2, 35}, {3, 100}}},
////        {1, {{0, 10}, {2, 15}}},
////        {2, {{0, 35}, {1, 15}}},
////        {3, {{0, 100}}}
////    };
//    vector<unordered_set<pair<int, int>, PHash, PCompare>> graph = {
//    {{{1, 10}, {2, 35}, {3, 100}}},    
//    {{{0, 10}, {2, 15}}},       
//    {{{0, 35}, {1, 15}}},       
//    {{{0, 100}}}           
//};
//    
//    
//
//    IncrementalDynamicSSSP sssp(3, 1, graph, 100, 1000);
//    cout<<"t"<<sssp.query(0, 1)<<endl;
//    sssp.insert(1, 0, 1);
//    
//    cout<<"t"<<sssp.query(0, 1)<<endl;
//    
//    cout<<"t"<<sssp.query(0, 2)<<endl;
//    cout<<"t"<<sssp.query(3, 2)<<endl;
//
//    return 0;
//}
