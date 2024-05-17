#include "DynamicHeap.hpp"

void DynamicHeap::push(pair<int,int> element)
{
    if(mapa.count(element.second) && mapa[element.second] == element.first) return;
    
    mapa[element.second] = element.first;
    pq.push(element);
}

void DynamicHeap::cleanupQueue()
{
    while(!pq.empty() && mapa[pq.top().second]!=pq.top().first)
    {
        int temp = pq.top().second;
        pq.pop();
        if(mapa.find(temp)!=mapa.end())
            pq.push({mapa[temp], temp});
    }
}

void DynamicHeap::pop()
{
    cleanupQueue();
    
    if(pq.empty()) return;
    auto temp = pq.top();
    pq.pop();
    mapa.erase(temp.second);
}

void DynamicHeap::changeFirst(int value, int key)
{
    mapa[key] = value;
}

pair<int, int> DynamicHeap::top()
{
    cleanupQueue();
    if(pq.empty()) return {-1, -1};
    return pq.top();
}
