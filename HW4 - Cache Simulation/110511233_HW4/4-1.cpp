#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N, M, cache_misses = 0;
    cin >> N >> M;

    vector<int> dmc(N, -1);

    for (int i = 0, tmp; i < M; i++) {
        cin >> tmp;
        if (find(dmc.begin(), dmc.end(), tmp) == dmc.end()) {
            cache_misses += 1;
            dmc[i % N] = tmp;
        }
    }

    cout << "Total Cache Misses:" << cache_misses << endl;

    return 0;
}