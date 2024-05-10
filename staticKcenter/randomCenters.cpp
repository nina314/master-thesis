#include "randomCenters.hpp"
#include <queue>
#include <iostream>
#include <cstdlib>
#include <ctime>   
#include <unordered_set>
#include <vector>

using namespace std;

vector<int> randomCenters(vector<unordered_set<pair<int, int>, PHash, PCompare>>& graph, int k) {
    int n = graph.size();
    vector<int> numbers(n);
        
    for (int i = 0; i < n; ++i) {
        numbers[i] = i;
    }

    vector<int> result(k);
    int lastIndex = n - 1;
    
    for (int i = 0; i < k; ++i) {
        int randomIndex = rand() % (lastIndex + 1);
        result[i] = numbers[randomIndex];
        swap(numbers[randomIndex], numbers[lastIndex]);
        --lastIndex;
    }

    return result;
}
