struct process_details *sort_by_arrival_time(struct process_details *processes, int size);
void print_results(struct process_details *processes,
                   struct process_details *ppsq_results,
                   struct process_details *ppsa_results,
                   struct process_details *rrsq_results,
                   int size);