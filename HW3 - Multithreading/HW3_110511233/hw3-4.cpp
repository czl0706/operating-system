#include <iostream>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#define MAX_THREADS 8
using namespace std;

int n, m;
int num_threads = 1, start_idx;
thread t[MAX_THREADS];

vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0; 

// uint64_t solve(int index, uint64_t current) {
//     if (index == m) {
//         if (current == (one << n) - 1) return 1;
//     } else {
//         return solve(index + 1, current) + solve(index + 1, current | subsets[index]);
//     }
//     return 0;
// }

uint64_t solve(int index, uint64_t current) {
    if (index == m) {
        return current == (one << n) - 1;
    } 
    return solve(index + 1, current) + solve(index + 1, current | subsets[index]);
}

void thread_solve(int thread_id) {
    static mutex gc_mutex;
    uint64_t start = 0;
    int index = 0;
    while (thread_id > 0) {
        start |= (thread_id & 1) ? subsets[index] : 0;
        thread_id >>= 1;
        index += 1;
    }
    uint64_t local_count = solve(start_idx, start);

    {
        lock_guard<mutex> lock(gc_mutex);
        global_count += local_count;
    }
}

int main(int argc, char* argv[]) {
    if (strcmp(argv[1], "-t") == 0) { num_threads = atoi(argv[2]); }
    
    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    start_idx = log2(num_threads);
    if (start_idx >= m) {
        start_idx = log2(m);
        num_threads = 1 << start_idx;
    }

    for (int i = 0; i < num_threads; i++) {
        t[i] = thread(thread_solve, i);
    }

    for (int i = 0; i < num_threads; i++) {
        t[i].join();
    }

    cout << global_count << endl;
    return 0;
}
