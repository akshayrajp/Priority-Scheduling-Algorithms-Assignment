#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "read_csv.h"
#include "process_details.h"
#include "common_macros.h"
#include "helpers.h"

#include "ppsqa.h"

int main()
{
    int i;
    struct process_details *processes = read_csv();
    int line_count = get_line_count();
    struct process_details ppsq_processes_copy[line_count],
        ppsa_processes_copy[line_count];

    for (i = 0; i < line_count; i++)
    {
        ppsq_processes_copy[i] = processes[i];
        ppsa_processes_copy[i] = processes[i];
    }

    struct process_details *ppsq_results = preemptive_process_scheduling_quantum_or_aging(ppsq_processes_copy, line_count, TYPE_QUANTUM);
    struct process_details *ppsa_results = preemptive_process_scheduling_quantum_or_aging(ppsa_processes_copy, line_count, TYPE_AGING);
    print_results(ppsq_results, ppsa_results, line_count);
    return 0;
}