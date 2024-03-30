#include <queue>
#include <unordered_map>

using namespace std;

class DynamicHeap
{
    public:
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    unordered_map<int, int> mapa;
    
    void push(pair<int, int> element);
    void pop();
    void changeFirst(int first, int second);
    void cleanupQueue();
    pair<int, int> top();
};