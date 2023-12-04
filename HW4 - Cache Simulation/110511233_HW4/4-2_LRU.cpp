#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
using namespace std;

int main() {
    int M, cache_misses = 0;
    unsigned int N, K;
    cin >> N >> K >> M;

    vector<list<int>> k_way(N / K, list<int>());
    list<int>::iterator it;

    for (int i = 0, tmp; i < M; i++) {
        cin >> tmp;

        vector<list<int>>::iterator v_it;
        for (v_it = k_way.begin(); v_it != k_way.end(); v_it++) {
            if ((it = find(v_it->begin(), v_it->end(), tmp)) != v_it->end()) {
                v_it->erase(it);
                v_it->push_back(tmp);
                break;
            }
        }
        if (v_it == k_way.end()) {
            int idx = i % (N / K);
            cache_misses += 1;
            if (k_way[idx].size() == K) {
                k_way[idx].pop_front();
            }
            k_way[idx].push_back(tmp);
        }
    }
    cout << "Total Cache Misses:" << cache_misses << endl;
    return 0;
}