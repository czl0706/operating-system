#include <iostream>
#include <cstring>
#include <thread>
#define MAX_THREADS 8
using namespace std;

int n, total_count = 0;
long num_threads = 1;
int count[MAX_THREADS] = {0};
thread t[MAX_THREADS];

bool is_prime(int num) {
    if (num == 1) return false;

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void count_primes(int thread_id) {
    for (int i = thread_id + 1; i <= n; i += num_threads) {
        if (is_prime(i)) count[thread_id] += 1;
    }
}

int main(int argc, char* argv[]) {

    if (strcmp(argv[1], "-t") == 0) { num_threads = atoi(argv[2]); }

    cin >> n;
    
    for (int i = 0; i < num_threads; i++) {
        t[i] = thread(static_cast<void(*)(int)>(count_primes), i);
    }
    for (int i = 0; i < num_threads; i++) {
        t[i].join();
        total_count += count[i];
    }

    cout << total_count << endl;
    return 0;
}
