#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_NAME_LEN 20
#define MAX_PROCESSES 5

// Process structure
typedef struct {
    char pid[MAX_NAME_LEN];        // process ID
    int arrival_time;               // Arrival time (ms)
    int run_time;                   // Total CPU time (ms) needed
    int remaining_time;             // Remaining execution time (for RR)
    int initial_schedule_time;      // Initial schedule time by CPU [You will need to keep track of it for response time calculation]
    int completion_time;            // Completion time of a process [You will need to keep track of it for tunraround time calculation]
} Process;


// Function Prototypes
void run_fcfs(int argc, char *argv[]);
void run_rr(int argc, char *argv[]);

void usage(char *str); // already implemented in tester.c

#endif
