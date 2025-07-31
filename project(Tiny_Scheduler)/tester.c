#include "utils.h"

/**
 * Program: TINY SCHEDULER
 * Description: 
 *   This program simulates process scheduling using the First-Come, First-Served (FCFS) 
 *   and Round Robin (RR) scheduling algorithms.
 *
 * Usage:
 *   ./tsim fcfs|rr input_file.txt  [time_slice]
 *
 * Example Usage:
 *   ./tsim fcfs input1.txt 
 *   ./tsim rr 3 input2.txt 
 */


void usage(char *str) {
    printf("%s\n", str);
    fprintf(stderr, "Usage: ./tsim fcfs|rr input_file.txt [time_slice(1-5)ms]\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        if (strcmp(argv[1], "fcfs") == 0) {
            run_fcfs(argc, argv);
        } else if (strcmp(argv[1], "rr") == 0) {
            run_rr(argc, argv);
        } else {
            usage("Error: Invalid scheduling policy.");
        }
    }

    return 0;
}
