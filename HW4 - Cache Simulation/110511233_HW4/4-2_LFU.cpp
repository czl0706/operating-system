#include <algorithm>
#include <iostream>
#include <list>
#include <utility>
#include <vector>
using namespace std;

int main() {
    using pii = pair<int, int>;

    int M, cache_misses = 0;
    unsigned int N, K;
    cin >> N >> K >> M;

    vector<list<pii>> k_way(N / K, list<pii>());
    list<pii>::iterator it;

    for (int i = 0, tmp; i < M; i++) {
        cin >> tmp;

        vector<list<pii>>::iterator v_it;
        for (v_it = k_way.begin(); v_it != k_way.end(); v_it++) {
            if ((it = find_if(v_it->begin(), v_it->end(), [tmp](const pii &a) { return a.first == tmp; })) != v_it->end()) {
                it->second += 1;
                break;
            }
        }
        if (v_it == k_way.end()) {
            int idx = i % (N / K);
            cache_misses += 1;

            pii x(tmp, 1);

            if (k_way[idx].size() == K) {
                auto min_it = min_element(k_way[idx].begin(), k_way[idx].end(),
                                          [](const pii &a, const pii &b) { return a.second < b.second; });
                *min_it = x;
            } else {
                k_way[idx].push_back(x);
            }
        }
    }
    cout << "Total Cache Misses:" << cache_misses << endl;
    return 0;
}