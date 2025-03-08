#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define N 50

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int rem_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int response_time;
    int priority;
} Process;
void generate_processes(Process p[]) {
    for (int i = 0; i < N; i++) {
        p[i].pid = i + 1;
        p[i].arrival_time = rand() % 50;
        p[i].burst_time = (rand() % 10) + 1; 
        p[i].rem_time = p[i].burst_time;
    }
}
void print_processes(Process p[]) {
    printf("\n%-5s %-10s %-10s %-10s %-10s %-10s %-10s\n", "PID", "Arrival", "Start", "Burst", "TAT", "RT", "Completion");
    for (int i = 0; i < N; i++) {
        printf("%-5d %-10d %-10d %-10d %-10d %-10d %-10d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].start_time,
               p[i].burst_time,
               p[i].turnaround_time,
               p[i].response_time,
               p[i].completion_time);
    }
}
static inline int compare_arrival(const void *a, const void *b) {
    return ((Process*)a) ->arrival_time - ((Process*)b)->arrival_time;
}
//Shortest Job First
void sjn(Process p[]){
    int finished=0,time=0;
    bool vis[N]={0};
    while(finished<N){
        int ind=-1,mn=101;
        for(int i=0;i<N;i++){
            if(!vis[i]&&p[i].arrival_time<=time){
                if(p[i].burst_time<mn){
                    mn=p[i].burst_time,ind=i;
                }
            }
        }
        if(ind==-1){
            time++;
            continue;
        }
        p[ind].start_time=time;
        p[ind].response_time=p[ind].start_time-p[ind].arrival_time;
        time+=p[ind].burst_time;
        p[ind].completion_time=time;
        p[ind].turnaround_time=p[ind].completion_time-p[ind].arrival_time;
        vis[ind]=true;
        finished++;
    }
}
//First Come First Serve
void fcfs(Process p[]){
    int time=0;
    for(int i=0;i<N;i++){
        if(time < p[i].arrival_time)time=p[i].arrival_time;
        p[i].start_time=time;
        p[i].response_time=p[i].start_time-p[i].arrival_time;
        time+=p[i].burst_time;
        p[i].completion_time=time;
        p[i].turnaround_time=p[i].completion_time-p[i].arrival_time;
    }
}
//Round Robin
inline void resize_queue(int *q,int capacity) {
    capacity *= 2;
    q = (int*)realloc(q, capacity * sizeof(int));
}
void rr(Process p[],int quanta){
    int time=0,fin=0;
    int l=0,r=0,capacity = N;
    int *q = (int*)malloc(N * sizeof(int));
    bool *inq = (bool*)calloc(N, sizeof(bool));

    while(fin<N){
        if(l==r){
            time++;
            for(int i=0;i<N;i++){
                if(p[i].arrival_time==time&&!inq[i]&&p[i].rem_time>0){
                    if (r >= capacity) {
                        capacity *= 2;
                        q = (int*)realloc(q, capacity * sizeof(int));
                    }
                    q[r++]=i;
                    inq[i]=1;
                }
            }
            continue;
        }
        int ind=q[l++];
        if(p[ind].rem_time==p[ind].burst_time){
            p[ind].start_time=time;
            p[ind].response_time=time-p[ind].arrival_time;
        }
        int ext=(p[ind].rem_time<quanta?p[ind].rem_time:quanta);
        p[ind].rem_time-=ext;
        time+=ext;
        for(int i=0;i<N;i++){
            if(p[i].arrival_time<=time&&!inq[i]){
                if (r >= capacity) {
                    capacity *= 2;
                    q = (int*)realloc(q, capacity * sizeof(int));
                }
                q[r++]=i;
                inq[i]=1;
            }
        }
        if(p[ind].rem_time>0){
            if (r >= capacity) {
                capacity *= 2;
                q = (int*)realloc(q, capacity * sizeof(int));
            }
            q[r++]=ind,inq[ind]=1;
        }
        else {
            p[ind].completion_time=time;
            p[ind].turnaround_time=p[ind].completion_time-p[ind].arrival_time;
            fin++;
            inq[ind]=0;
        }
    }
    free(q);
    free(inq);
}
//Priority Scheduling
static inline int compare_priority(const void *a, const void *b) {
    return ((Process*)a) ->priority - ((Process*)b)->priority;
}
void prior(Process p[]){
    for(int i=0;i<N;i++){
        p[i].priority = rand() % 50;
    }
    qsort(p,N,sizeof(Process),compare_priority);
    int time=0,fin=0;
    bool vis[N]={0};
    while(fin<N){
        int ind=-1,mn=101;
        for(int i=0;i<N;i++){
            if(!vis[i]&&p[i].arrival_time<=time){
                if(p[i].priority<mn){
                    mn=p[i].priority,ind=i;
                }
            }
        }
        if(ind==-1){
            time++;
            continue;
        }
        p[ind].start_time=time;
        p[ind].response_time=p[ind].start_time-p[ind].arrival_time;
        time+=p[ind].burst_time;
        p[ind].completion_time=time;
        p[ind].turnaround_time=p[ind].completion_time-p[ind].arrival_time;
        vis[ind]=true;
        fin++;
    }
}
int main(){
    Process p[N];
    generate_processes(p);
    FILE *f = fopen("output.txt", "w");
    qsort(p, N, sizeof(Process), compare_arrival);
    sjn(p);
    double total_tat[4] = {0.0}, total_rt[4] = {0.0};
    for (int i = 0; i < N; i++) {
        total_tat[0] += p[i].turnaround_time;
        total_rt[0] += p[i].response_time;
    }
    fprintf(f, "Scheduling Algorithm :SJN:\nAverage Turnaround Time: %.2f\nAverage Response Time: %.2f\n",
             total_tat[0] / N, total_rt[0] / N);
    // print_processes(p);
    fcfs(p);
    for (int i = 0; i < N; i++) {
        total_tat[1] += p[i].turnaround_time;
        total_rt[1] += p[i].response_time;
    }
    fprintf(f, "Scheduling Algorithm :FCFS:\nAverage Turnaround Time: %.2f\nAverage Response Time: %.2f\n",
             total_tat[1] / N, total_rt[1] / N);
    // print_processes(p);
    rr(p,5);
    for (int i = 0; i < N; i++) {
        total_tat[2] += p[i].turnaround_time;
        total_rt[2] += p[i].response_time;
    }
    fprintf(f, "Scheduling Algorithm :RR:\nAverage Turnaround Time: %.2f\nAverage Response Time: %.2f\n",
             total_tat[2] / N, total_rt[2] / N);
    // print_processes(p);
    prior(p);
    for (int i = 0; i < N; i++) {
        total_tat[3] += p[i].turnaround_time;
        total_rt[3] += p[i].response_time;
    }
    fprintf(f, "Scheduling Algorithm :Priority:\nAverage Turnaround Time: %.2f\nAverage Response Time: %.2f\n",
             total_tat[3] / N, total_rt[3] / N);
}
