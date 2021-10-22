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

void print_results(struct process_details *processes,
                   struct process_details *ppsq_results,
                   struct process_details *ppsa_results,
                   struct process_details *rrsq_results,
                   int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (processes[i].process_id == ppsq_results[j].process_id)
                printf("PPS,process id '%c',%d,%d,%d,%d\n",
                       ppsq_results[j].process_id,
                       ppsq_results[j].pps.completion_time,
                       ppsq_results[j].pps.turnaround_time,
                       ppsq_results[j].pps.waiting_time,
                       ppsq_results[j].pps.response_time);
        }
        for (int j = 0; j < size; j++)
        {
            if (processes[i].process_id == ppsa_results[j].process_id)
                printf("PPA,process id '%c',%d,%d,%d,%d\n",
                       ppsa_results[j].process_id,
                       ppsa_results[j].ppa.completion_time,
                       ppsa_results[j].ppa.turnaround_time,
                       ppsa_results[j].ppa.waiting_time,
                       ppsa_results[j].ppa.response_time);
        }
        for (int j = 0; j < size; j++)
        {
            if (processes[i].process_id == rrsq_results[j].process_id)
                printf("RRS,process id '%c',%d,%d,%d,%d\n",
                       rrsq_results[j].process_id,
                       rrsq_results[j].rrs.completion_time,
                       rrsq_results[j].rrs.turnaround_time,
                       rrsq_results[j].rrs.waiting_time,
                       rrsq_results[j].rrs.response_time);
        }
    }
}

void print_contents(struct process_details *processes, int size)
{
    // printf("Process ID\tCompletion Time\tTurnaround Time\tWaiting Time\tResponse Time\n\n");
    // for (int i = 0; i < size - 1; i++)
    // {
    //     printf("%c\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].process_id,
    //            processes[i].rrs.completion_time,
    //            processes[i].rrs.turnaround_time,
    //            processes[i].rrs.waiting_time,
    //            processes[i].rrs.response_time);
    // }
    for (int i = 0; i < size; i++)
        printf("%c", processes[i].process_id);
    puts("");
}