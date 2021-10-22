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
        for (int i = FRONT + 1; i < rqcount; i++)
            new_ready_queue[i - 1] = ready_queue[i];
        return new_ready_queue;
    }
    else if (strcmp(operation, OPERATION_ENQUEUE) == 0)
    {
        struct process_details *new_ready_queue = (struct process_details *)malloc(sizeof(struct process_details) * rqcount);
        new_ready_queue[rqcount - 1] = ready_queue[FRONT];
        for (int i = FRONT + 1; i < rqcount; i++)
            new_ready_queue[i - 1] = ready_queue[i];
        return new_ready_queue;
    }
}
struct process_details *round_robin_scheduling_quantum(struct process_details *processes, int size)
{
    int i, j, time_counter = 0, done_count = 0, pcount = 0, rqcount = 0, count = 0;
    char process_to_be_executed;
    bool done_flag = false;
    struct process_details results[size];

    // sort the procceses by arrival_time
    processes = sort_by_arrival_time(processes, size);

    struct process_details *ready_queue = (struct process_details *)malloc(sizeof(struct process_details) * FRONT);
    struct process_details *temp_ready_queue = (struct process_details *)malloc(sizeof(struct process_details) * FRONT);
    struct process_details *new_ready_queue = (struct process_details *)malloc(sizeof(struct process_details) * FRONT);

    // set is_completed of all processes to false, is_new to true and first_insertion to -1
    for (i = 0; i < size; i++)
    {
        processes[i].is_completed = false;
        processes[i].is_new = true;
        processes[i].first_insertion = -1;
    }

    // Add processes to the ready queue based on whether they have arrived or not
    // Make sure that the most recently arrived process is first in the queue
    while (true)
    {
        pcount = 0;
        char *p_ids = (char *)malloc(sizeof(char) * (pcount + 1));
        for (i = 0; i < size; i++)
        {
            if (!processes[i].is_completed &&
                processes[i].arrival_time <= time_counter &&
                processes[i].is_new)
            {
                processes[i].is_new = false;
                p_ids[pcount] = processes[i].process_id;
                count += 1;
                pcount += 1;
                p_ids = (char *)realloc(p_ids, sizeof(char) * (pcount + 1));
            }
        }
        // so now, ready_queue needs 'count' number of elements
        if (pcount > 0)
        {
            ready_queue = (struct process_details *)realloc(ready_queue, sizeof(struct process_details) * count);
            temp_ready_queue = (struct process_details *)realloc(temp_ready_queue, sizeof(struct process_details) * (count - pcount));
            new_ready_queue = (struct process_details *)realloc(new_ready_queue, sizeof(struct process_details) * pcount);

            // store the contents of ready_queue in temp_ready_queue
            for (i = 0; i < count - pcount; i++)
                temp_ready_queue[i] = ready_queue[i];

            // store the newer (most recently arrived) processes in new_ready_queue
            for (i = 0; i < pcount; i++)
                for (j = 0; j < size; j++)
                    if (processes[j].process_id == p_ids[i])
                        new_ready_queue[i] = processes[j];

            // Now, ready_queue = new_ready_queue + temp_ready_queue
            for (i = 0; i < pcount; i++)
                ready_queue[i] = new_ready_queue[i];
            for (i = pcount; i < count; i++)
                ready_queue[i] = temp_ready_queue[i - pcount];

            // free p_ids as we no longer have any use for it
            free(p_ids);
        }

        // now, we need to execute the process that is at the FRONT of the queue

        ready_queue[FRONT].is_active = true;
        ready_queue[FRONT].is_waiting = false;
        if (ready_queue[FRONT].first_insertion == -1)
            ready_queue[FRONT].first_insertion = time_counter;
        time_counter += MIN(QUANTUM, ready_queue[FRONT].burst_length);
        ready_queue[FRONT].burst_length = MAX(0, ready_queue[FRONT].burst_length - QUANTUM);

        // if the process has finished executing, then calculate the required parameters,
        // mark it as completed, and remove it from the process_queue
        if (ready_queue[FRONT].burst_length == 0)
        {
            ready_queue[FRONT].is_completed = true;
            ready_queue[FRONT].is_active = false;
            ready_queue[FRONT].is_waiting = false;

            ready_queue[FRONT].rrs.completion_time = time_counter;

            ready_queue[FRONT].rrs.turnaround_time = time_counter -
                                                     ready_queue[FRONT].arrival_time;

            ready_queue[FRONT].rrs.waiting_time = ready_queue[FRONT].rrs.turnaround_time -
                                                  ready_queue[FRONT].burst_length_copy;

            ready_queue[FRONT].rrs.response_time = ready_queue[FRONT].first_insertion -
                                                   ready_queue[FRONT].arrival_time;

            for (i = 0; i < size; i++)
                if (ready_queue[FRONT].process_id == processes[i].process_id)
                {
                    processes[i] = ready_queue[FRONT];
                    break;
                }

            done_count += 1;
            new_ready_queue = (struct process_details *)realloc(new_ready_queue, sizeof(struct process_details) * (count - 1));
            for (i = FRONT + 1; i < count; i++)
                new_ready_queue[i - 1] = ready_queue[i];
            ready_queue = (struct process_details *)realloc(ready_queue, sizeof(struct process_details) * (count - 1));
            count -= 1;

            for (i = 0; i < count; i++)
                ready_queue[i] = new_ready_queue[i];
        }
        else
        {
            // However, if the process has not finished it's execution, then add it to the back of the queue
            new_ready_queue = (struct process_details *)realloc(new_ready_queue, sizeof(struct process_details) * count);
            new_ready_queue[count - 1] = ready_queue[FRONT];
            for (i = FRONT + 1; i < count; i++)
                new_ready_queue[i - 1] = ready_queue[i];

            ready_queue = (struct process_details *)realloc(ready_queue, sizeof(struct process_details) * count);

            for (i = 0; i < count; i++)
                ready_queue[i] = new_ready_queue[i];
        }
        if (done_count == size)
        {
            free(ready_queue);
            free(new_ready_queue);
            free(temp_ready_queue);
            return sort_by_arrival_time(processes, size);
        }
    }
}
