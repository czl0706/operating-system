#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>

using namespace std;

sem_t sem;

void count(int index) {
  static mutex io_mutex;
  int num = 1000000;
  while (num--) {}
  {
    int sval;
    do { sem_getvalue(&sem, &sval); } while (sval != index);
    lock_guard<mutex> lock(io_mutex);
    cout << "I'm thread " << index << ", local count: 1000000\n";
    sem_post(&sem);
  }
}

int main(void) {
  thread t[100];
  sem_init(&sem, 0, 0);

  for (int i = 0; i < 100; i++)
    t[i] = thread(static_cast<void(*)(int)>(count), i);

  for (int i = 0; i < 100; i++)
    t[i].join();
}
