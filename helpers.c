#include <stdio.h>

#include "common_macros.h"
#include "process_details.h"

struct process_details *sort_by_arrival_time(struct process_details *processes, int size)
{
    int i, j;
    struct process_details process;
    for (i = 0; i < size - 1; i++)
    {
        for (j = i + 1; j < size; j++)
        {
            if (processes[i].arrival_time > processes[j].arrival_time)
            {
                process = processes[i];
                processes[i] = processes[j];
                processes[j] = process;
            }
        }
    }
    return processes;
}

void print_results(struct process_details *ppsq_results, struct process_details *ppsa_results, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("PPS,process id '%c',%d,%d,%d,%d\n",
               ppsq_results[i].process_id,
               ppsq_results[i].pps.completion_time,
               ppsq_results[i].pps.turnaround_time,
               ppsq_results[i].pps.waiting_time,
               ppsq_results[i].pps.response_time);
        printf("PPA,process id '%c',%d,%d,%d,%d\n",
               ppsa_results[i].process_id,
               ppsa_results[i].ppa.completion_time,
               ppsa_results[i].ppa.turnaround_time,
               ppsa_results[i].ppa.waiting_time,
               ppsa_results[i].ppa.response_time);
        printf("RRS,process id '%c',%d,%d,%d,%d\n",
               ppsq_results[i].process_id,
               ppsq_results[i].pps.completion_time,
               ppsq_results[i].pps.turnaround_time,
               ppsq_results[i].pps.waiting_time,
               ppsq_results[i].pps.response_time);
    }
}