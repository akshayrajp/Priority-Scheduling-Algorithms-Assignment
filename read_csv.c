#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "process_details.h"
#include "common_macros.h"

int get_line_count()
{
    FILE *fp;
    int line_count = 0;
    char c;

    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return (-1);
    }

    // count total number of lines in the given input csv file
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // increment count if this character is newline
            ++line_count;
    fclose(fp);

    return line_count + 1;
}
struct process_details *read_csv()
{
    FILE *fp;
    char str[1000], c;
    int line_count = 0, counter = 0, i;

    //  open the csv file for reading
    fp = fopen(INPUT_FILE, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
    }

    line_count = get_line_count(INPUT_FILE);
    struct process_details *processes;
    processes = malloc(sizeof(struct process_details) * line_count);

    // Process one line at a time
    while (fgets(str, 1000, fp) != NULL)
    {
        // Use strtok to tokenize the string using a specified delimiter (,)
        int init_size = strlen(str);
        char delim[] = ",";
        char *ptr = strtok(str, delim);

        // struct process_details p;
        int token_count = 0;
        char *token_array[4]; // Since we have four fields per line in the csv file
        while (ptr != NULL)
        {
            token_array[token_count++] = ptr;
            ptr = strtok(NULL, delim);
        }

        // Store the various details of the process in p
        processes[counter].process_id = *token_array[0];
        processes[counter].arrival_time = atoi(token_array[1]);
        processes[counter].burst_length = atoi(token_array[2]);
        processes[counter].burst_length_copy = atoi(token_array[2]);
        processes[counter].first_insertion = -1;
        processes[counter].priority = atoi(token_array[3]);

        counter++;
        free(ptr);
    }
    fclose(fp);
    return processes;
}