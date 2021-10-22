#include "common_macros.h"
#include "process_details.h"
#include "helpers.h"
#include <stdio.h>
#include <string.h>
struct process_details *preemptive_process_scheduling_quantum_or_aging(struct process_details *processes, int size, char *type)
{
    int i, time_counter = 0, max_priority = 0;
    char process_to_be_executed;
    bool done_flag = false;
    // Sort the procceses by arrival_time
    processes = sort_by_arrival_time(processes, size);

    // Set initial states
    for (i = 0; i < size; i++)
    {
        processes[i].is_completed = false;
        processes[i].first_insertion = -1;
    }

    while (!done_flag)
    {
        max_priority = 0;
        for (int i = 0; i < size; i++)
        {
            // if a process has not been completed yet
            // and if that process' arrival time is before or at the arrival time
            // and if it's priority is highest among all such processes
            // then that is the process that we shall be executing
            if (!processes[i].is_completed &&
                processes[i].arrival_time <= time_counter &&
                processes[i].priority > max_priority)
            {
                max_priority = processes[i].priority;
                process_to_be_executed = processes[i].process_id;
            }
        }
        for (i = 0; i < size; i++)
        {
            if (processes[i].process_id == process_to_be_executed)
            {
                // set flags of process to be executed to appropriate values
                processes[i].is_waiting = false;
                processes[i].is_active = true;

                // if the process has come into the ready queue for the first time
                // then update its insertion time
                if (processes[i].first_insertion == -1)
                    processes[i].first_insertion = time_counter;

                // for ppsq, time_counter is incremented either by QUANTUM
                // or by burst_length if it is less than QUANTUM
                // also, update the burst length of that process
                if (strcmp(type, TYPE_QUANTUM) == 0)
                {
                    time_counter += MIN(QUANTUM, processes[i].burst_length);
                    processes[i].burst_length = MAX(0, processes[i].burst_length - QUANTUM);
                }

                // for ppsa, time_counter is incremented by one
                else if (strcmp(type, TYPE_AGING) == 0)
                {
                    time_counter += 1;
                    processes[i].burst_length = MAX(0, processes[i].burst_length - AGING_PERIOD);
                }

                // if a process has finished execution i.e. if its burst_length == 0
                // then update the flag to appropriate values
                if (processes[i].burst_length == 0)
                {
                    processes[i].is_completed = true;
                    processes[i].is_waiting = false;
                    processes[i].is_active = false;

                    // perform necessary calculations for the finished process
                    if (strcmp(type, TYPE_QUANTUM) == 0)
                    {
                        processes[i].pps.completion_time = time_counter;

                        processes[i].pps.turnaround_time = time_counter -
                                                           processes[i].arrival_time;

                        processes[i].pps.waiting_time = processes[i].pps.turnaround_time -
                                                        processes[i].burst_length_copy;

                        processes[i].pps.response_time = processes[i].first_insertion -
                                                         processes[i].arrival_time;
                    }
                    else if (strcmp(type, TYPE_AGING) == 0)
                    {
                        processes[i].ppa.completion_time = time_counter;

                        processes[i].ppa.turnaround_time = time_counter -
                                                           processes[i].arrival_time;

                        processes[i].ppa.waiting_time = processes[i].ppa.turnaround_time -
                                                        processes[i].burst_length_copy;

                        processes[i].ppa.response_time = processes[i].first_insertion -
                                                         processes[i].arrival_time;
                    }
                }
                // if the process has not finished, pre-empt it
                // and give another process a chance
                // by setting this process' flags to appropriate values
                else
                {
                    processes[i].is_waiting = true;
                    processes[i].is_active = false;
                }
            }
            // for the other processes that have been added to the ready queue
            // but are not in active mode, increment their priority if ppsa

            // irrespective of whether the algorithm is ppsq or ppsa,
            // update the process' flags to appropriate values
            else if (processes[i].process_id != process_to_be_executed &&
                     processes[i].arrival_time <= time_counter &&
                     !processes[i].is_completed)
            {
                processes[i].is_waiting = true;
                processes[i].is_active = false;
                if (strcmp(type, TYPE_AGING) == 0)
                    processes[i].priority += AGING_PERIOD;
            }
        }

        // if all the proccesses have finished executing, set done_flag to true
        for (i = 0; i < size; i++)
        {
            if (!processes[i].is_completed)
            {
                done_flag = false;
                break;
            }
            else
                done_flag = true;
        }
    }
    return processes;
}