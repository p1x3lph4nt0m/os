#include <stdio.h>
#include <stdlib.h>

#define N 500 
#define INTERVALS 4

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int base_priority;
    int nice;
    double cpu_util[INTERVALS];  
    double priority[INTERVALS];  
    int start_time;
    int completion_time;
    int turnaround_time;
    int response_time;
} Process;

void generate_processes(Process p[]) {
    for (int i = 0; i < N; i++) {
        p[i].pid = i + 1;
        p[i].arrival_time = rand() % 10;
        p[i].burst_time = (rand() % 20) + 1;
        p[i].remaining_time = p[i].burst_time;
        p[i].base_priority = (rand() % 50) + 1;  // Random base priority
        p[i].nice = (rand() % 41) - 20;  // Nice value from -20 to +20
        p[i].cpu_util[0] = rand() % 100;  // Initial CPU utilization

        p[i].priority[0] = p[i].base_priority + (p[i].cpu_util[0] / 2.0) + p[i].nice;
        for (int j = 1; j < INTERVALS; j++) {
            p[i].cpu_util[j] = p[i].cpu_util[j - 1] / 2.0;
            p[i].priority[j] = p[i].base_priority + (p[i].cpu_util[j] / 2.0) + p[i].nice;
        }
    }
}

int compare_priority(const void *a, const void *b) {
    Process *p1 = (Process*)a;
    Process *p2 = (Process*)b;
    return (p2->priority[0] - p1->priority[0]) > 0 ? 1 : -1;
}

// Priority Scheduling Algorithm
void priority_scheduling(Process p[]) {
    int time = 0, completed = 0;
    
    while (completed < N) {
        qsort(p, N, sizeof(Process), compare_priority);

        int index = -1;
        for (int i = 0; i < N; i++) {
            if (p[i].arrival_time <= time && p[i].remaining_time > 0) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            time++;
            continue;
        }

        if (p[index].remaining_time == p[index].burst_time) {
            p[index].start_time = time;
            p[index].response_time = p[index].start_time - p[index].arrival_time;
        }

        time += p[index].remaining_time;
        p[index].completion_time = time;
        p[index].turnaround_time = p[index].completion_time - p[index].arrival_time;
        p[index].remaining_time = 0;  // Process completed
        completed++;

        for (int i = 0; i < N; i++) {
            if (p[i].remaining_time > 0) {
                for (int j = 1; j < INTERVALS; j++) {
                    p[i].cpu_util[j] = p[i].cpu_util[j - 1] / 2.0;
                    p[i].priority[j] = p[i].base_priority + (p[i].cpu_util[j] / 2.0) + p[i].nice;
                }
            }
        }
    }
}

void print_processes(Process p[]) {
    printf("\n%-5s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n",
           "PID", "Arrival", "Burst", "Priority-0","Priority-1","Priority-2","Priority-3", "TAT", "RT", "Completion", "CPU[0]");
    for (int i = 0; i < N; i++) {
        printf("%-5d %-10d %-10d %-10.6f %-10.6f %-10.6f %-10.6f %-10d %-10d %-10d %-10.2f \n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].priority[0],p[i].priority[1],p[i].priority[2],p[i].priority[3], p[i].turnaround_time, p[i].response_time,
               p[i].completion_time, p[i].cpu_util[0]);
    }
}

// Main function
int main() {
    Process p[N];
    generate_processes(p);
    priority_scheduling(p);
    // print_processes(p);
    FILE *f = fopen("test.txt", "w");
    double total_tat = 0.0, total_rt = 0.0;
    for (int i = 0; i < N; i++) {
        total_tat += p[i].turnaround_time;
        total_rt += p[i].response_time;
    }
    fprintf(f, "Scheduling Algorithm :SJN:\nAverage Turnaround Time: %.2f\nAverage Response Time: %.2f\n",
             total_tat / N, total_rt / N);
    return 0;
}
