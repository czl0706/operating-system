#include <iostream>
#include <list>
using namespace std;

struct process {
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int remain_time;
    int queue_time;
    int run_time;
    int proc_id;
};

struct process_list {
    list<process*> queue;
    int time_quantum;
    int jobs_remaining;
    int mode;
};

enum mode { FCFS, SRTF, RR };

void queue_insertion(list<process_list>::iterator q_it, process* proc) {
    (q_it->jobs_remaining)++;
    if (q_it->queue.empty()) { q_it->queue.push_back(proc); return; }
    switch (q_it->mode) {
    case FCFS: { q_it->queue.push_back(proc); }
        break;
    case SRTF: {
        auto it = q_it->queue.rbegin();
        for (; it != q_it->queue.rend(); it++) {
            if (proc->remain_time > (*it)->remain_time || 
                (proc->remain_time == (*it)->remain_time && proc->arrival_time > (*it)->arrival_time)) {
                q_it->queue.insert(it.base(), proc);
                return;
            }
        }
        q_it->queue.insert(it.base(), proc);
    }
        break;
    case RR: {
        auto it = q_it->queue.rbegin();
        for (; it != q_it->queue.rend(); it++) {
            if (proc->queue_time > (*it)->queue_time || 
                (proc->queue_time == (*it)->queue_time && proc->arrival_time < (*it)->arrival_time)) {
                q_it->queue.insert(it.base(), proc);
                return;
            }
        }
        q_it->queue.insert(it.base(), proc);
    }
        break;
    default: break;
    }
}

bool check_preemption(list<process_list>::iterator q_it, process* proc) {
    if (q_it->queue.empty()) { return false; }
    switch (q_it->mode) {
    case FCFS: { return false; }
        break;
    case SRTF: {
        auto it = q_it->queue.begin();
        if (proc->remain_time > (*it)->remain_time || 
            (proc->remain_time == (*it)->remain_time && proc->arrival_time > (*it)->arrival_time)) {
            return false;
        }
        return true;
    }
        break;
    case RR: { return false; }
        break;
    default: break;
    }
    return false;
}

int main() {
    int N, M;
    cin >> N >> M;

    list<process_list> proc_list(N);
    process p[M];

    list<process_list>::iterator q_it = proc_list.begin();
    for (int i = 0; i < N; i++, q_it++) { 
        cin >> q_it->mode >> q_it->time_quantum;
        q_it->jobs_remaining = 0;
    }

    for (int i = 0, arrival_time, burst_time; i < M; i++) {
        cin >> arrival_time >> burst_time; 
        p[i] = {
            .arrival_time = arrival_time,
            .burst_time = burst_time,
            .waiting_time = 0,
            .turnaround_time = 0,
            .remain_time = burst_time,
            .queue_time = arrival_time,
            .run_time = 0,
            .proc_id = i
        };
    }

    // initialize with the first process
    q_it = proc_list.begin(); q_it->jobs_remaining = 1; q_it->queue.push_back(&p[0]);

    for (   int system_time = p[0].arrival_time, job_idx = 1, all_remaining = 1; 
            job_idx != M || all_remaining != 0; 
            system_time++) {
        process* proc = &p[job_idx];

        if (job_idx < M && system_time >= proc->arrival_time) {
            // corner case
            if (!q_it->queue.empty() && (q_it->queue.front())->remain_time == 0) {
                proc = q_it->queue.front();
                proc->turnaround_time = system_time - proc->arrival_time;
                proc->waiting_time = proc->turnaround_time - proc->burst_time;
                q_it->queue.pop_front();

                q_it->jobs_remaining--;
                all_remaining--;
                proc = &p[job_idx];
            }   

            // check for preemption 
            if (((q_it == proc_list.begin() && check_preemption(q_it, proc)) || (q_it != proc_list.begin() && q_it->jobs_remaining)) &&
                q_it->queue.front()->run_time != 0) {
                // preemption occurs
                process *x = q_it->queue.front(); q_it->queue.pop_front();
                q_it->jobs_remaining--;
                x->queue_time = system_time;
                x->run_time = 0;
                queue_insertion(next(q_it) == proc_list.end() ? q_it : next(q_it), x);
            }
            q_it = proc_list.begin();

            queue_insertion(q_it, proc);

            job_idx++;
            all_remaining++;
        } else if (q_it->queue.empty()) { continue; }

        proc = q_it->queue.front();

        if (proc->remain_time == 0) {
            proc->turnaround_time = system_time - proc->arrival_time;
            proc->waiting_time = proc->turnaround_time - proc->burst_time;
            q_it->queue.pop_front();
            q_it->jobs_remaining--;
            all_remaining--;

            // selects the next process
            if (!q_it->queue.empty()) { proc = q_it->queue.front(); }
        } else if (q_it->mode == RR && proc->run_time == q_it->time_quantum) {
            proc->run_time = 0;
            proc->queue_time = system_time;
            q_it->queue.pop_front();
            q_it->jobs_remaining--;
        
            queue_insertion(next(q_it) == proc_list.end() ? q_it : next(q_it), proc);

            // selects the next process
            if (!q_it->queue.empty()) { proc = q_it->queue.front(); }
        }

        // switch to the next queue
        while (q_it->jobs_remaining == 0 && q_it != proc_list.end()) { q_it++; }
        if (q_it == proc_list.end()) { q_it = proc_list.begin(); }
        if (!q_it->queue.empty()) { proc = q_it->queue.front(); }

        proc->run_time += 1;
        proc->remain_time -= 1;
    }

    int total_waiting = 0, total_turnaround = 0;
    for (int i = 0; i < M; i++) {
        cout << p[i].waiting_time << ' ' << p[i].turnaround_time << endl;
        total_waiting += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;
    }
    cout << total_waiting << endl 
         << total_turnaround << endl;

    return 0;
}