#include "DynamicHeap.hpp"

void DynamicHeap::push(pair<int,int> element)
{
    if(mapa[element.second] == element.first) return;
    
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
    auto temp = pq.top();
    pq.pop();
    mapa.erase(temp.second);
}

void DynamicHeap::changeFirst(int first, int second)
{
    mapa[second] = first;
}

pair<int, int> DynamicHeap::top()
{
    cleanupQueue();
    return pq.top();
}