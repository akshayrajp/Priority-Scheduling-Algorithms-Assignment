// A structure to store the results
#include <stdbool.h>
struct result
{
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
};

// A structure to contain the details of the process
struct process_details
{
    // properties of the process:
    char process_id;
    int arrival_time;
    int burst_length;
    int burst_length_copy;
    int first_insertion;
    int priority;

    // misc variables needed during the calculation:
    bool is_active;
    bool is_waiting;
    bool is_completed;

    // results for the process:
    struct result pps;
    struct result ppa;
    struct result rrs;
};