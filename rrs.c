#include "common_macros.h"
#include "process_details.h"
#include "helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct process_details *queue_operation(struct process_details *ready_queue, int rqcount, char *operation)
{
    if (strcmp(operation, OPERATION_DEQUEUE) == 0)
    {
        struct process_details *new_ready_queue = (struct process_details *)malloc(sizeof(struct process_details) * (rqcount - 1));
        for (int i = TOP + 1; i < rqcount - 1; i++)
            new_ready_queue[i - 1] = ready_queue[i];
        return new_ready_queue;
    }
    else if (strcmp(operation, OPERATION_ENQUEUE) == 0)
    {
        struct process_details *new_ready_queue = (struct process_details *)malloc(sizeof(struct process_details) * rqcount);
        new_ready_queue[rqcount - 1] = ready_queue[TOP];
        for (int i = TOP + 1; i < rqcount; i++)
            new_ready_queue[i - 1] = ready_queue[i];
        return new_ready_queue;
    }
}
struct process_details *round_robin_scheduling_quantum(struct process_details *processes, int size)
{
    int i, time_counter = 0, done_count = 0, pcount = 0, rqcount = 1;
    char process_to_be_executed;
    bool done_flag = false;
    // Sort the procceses by arrival_time
    processes = sort_by_arrival_time(processes, size);

    // set is_completed of all processes to false
    for (i = 0; i < size; i++)
        processes[i].is_completed = false;

    struct process_details *ready_queue = (struct process_details *)malloc(sizeof(struct process_details) * rqcount),
                           *results = (struct process_details *)malloc(sizeof(struct process_details) * size);
    while (true)
    {
        pcount = 0;
        char *pqueue = (char *)malloc(sizeof(char) * (pcount + 1)); // pcount + 1 to store the terminating '\0'
        // Add the process ids of processes which have arrived and haven't finished their execution
        // to the ready_queue
        for (i = 0; i < size; i++)
            if (!processes[i].is_completed && processes[i].arrival_time <= time_counter)
            {
                pqueue[pcount++] = processes[i].process_id;
                pqueue = realloc(pqueue, pcount + 1);
            }

        for (i = pcount - 1; i >= 0; i--)
        {
            for (int j = 0; j < size; j++)
                if (processes[j].process_id == pqueue[i])
                {
                    ready_queue = realloc(ready_queue, sizeof(struct process_details) * (++rqcount));
                    ready_queue[rqcount - 1] = processes[j];
                    break;
                }
        }

        ready_queue[TOP].is_active = true;
        ready_queue[TOP].is_waiting = false;
        time_counter += MIN(QUANTUM, ready_queue[TOP].burst_length);
        ready_queue[TOP].burst_length = MAX(0, ready_queue[TOP].burst_length - QUANTUM);
        if (ready_queue[TOP].first_insertion == -1)
            ready_queue[TOP].first_insertion = time_counter;

        if (ready_queue[TOP].burst_length == 0)
        {
            ready_queue[TOP].is_completed = true;
            ready_queue[TOP].is_active = false;
            ready_queue[TOP].is_waiting = false;

            ready_queue[TOP].rrs.completion_time = time_counter;

            ready_queue[TOP].rrs.turnaround_time = time_counter -
                                                   ready_queue[TOP].arrival_time;

            ready_queue[TOP].rrs.waiting_time = ready_queue[TOP].rrs.turnaround_time -
                                                ready_queue[TOP].burst_length_copy;

            ready_queue[TOP].rrs.response_time = ready_queue[TOP].first_insertion -
                                                 ready_queue[TOP].arrival_time;

            results[done_count++] = ready_queue[TOP];

            // Delete this process from ready_queue now
            ready_queue = queue_operation(ready_queue, rqcount, OPERATION_DEQUEUE);
        }
        // else if a process has not finished, then add it to the back of the queue
        else
            ready_queue = queue_operation(ready_queue, rqcount, OPERATION_ENQUEUE);

        if (done_count == size)
        {
            // free(ready_queue);
            return results;
        }
    }
}