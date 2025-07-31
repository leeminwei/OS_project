#include "stdio.h"
#include "utils.h"
#include "stdlib.h"
#include "string.h"

#define MAX_BUFFER 1024
#define MAX_NAME 1024
#define MAX_STRINGS 100
#define MAX_LEN 100
//實作queue的pop(整個字串陣列左移)
void queue_pop (char** arr, int *queue_index) {
    if (*queue_index < 0) return;

    // 左移 queue[1 2 ... n] -> queue[2 .. n-1]
    for (int i = 0; i < *queue_index; i++) {
        strcpy(arr[i], arr[i+1]);
    }
    (*queue_index)--;
}

char* pop (char** arr, int *queue_index) {
    if (*queue_index < 0) return NULL;

    //用來存queue要被pop出去的字串
    char *first = malloc(MAX_NAME);
    strcpy(first, arr[0]);

    // 左移 queue[1 2 ... n] -> queue[2 .. n-1]
    for (int i = 0; i < *queue_index; i++) {
        strcpy(arr[i], arr[i+1]);
    }
    (*queue_index)--;

    //返回被pop出來的pid
    return first;
}
char* peek (char** arr, int* queue_index) {
    if (*queue_index < 0) return NULL;
    return arr[0];
}

int push (char** arr, int index, const char* pid) {
    strcpy(arr[++index],pid);
    return index;
}

int compare_arrival_time(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrival_time - p2->arrival_time;
}

//用來找到當前running的是哪個processes
int find_process_index_by_pid (Process* processes, int total_processes, char* running) {
    int idx = 0;
    for (int i=0 ; i<total_processes ; i++) {
        if (strcmp(processes[i].pid, running) == 0) {
            idx = i;
        }
    }
    return idx;
}

void run_fcfs(int argc, char *argv[]) {
    FILE *input;
    char filename[100];
    char buffer[MAX_BUFFER];
    sprintf(filename, "%s", argv[2]);
    input = fopen(filename, "r");

    //如果open失敗
    if (input == NULL) {
        printf("No such file or directory\n");
    }

    //創建arr存分割的子字串
    char** arr = malloc(3 * sizeof(char*));
    for (int j = 0; j < 3; j++) {
        arr[j] = malloc(MAX_NAME);
    }
    //process結構
    Process processes[1024];
    int i = 0;

    //讀取文字檔的每一行並做字串分割存到processes
    while (fgets(buffer, MAX_BUFFER, input) != NULL) {
        //printf("%s", buffer);
        int arr_index = 0;
        char *token;
        token = strtok(buffer, " ");
        while(token != NULL){
            arr[arr_index++] = token;
            token = strtok(NULL," ");
        }
        strcpy(processes[i].pid, arr[0]);
        processes[i].arrival_time = atoi(arr[1]);
        processes[i].run_time = atoi(arr[2]);
        processes[i].remaining_time = processes[i].run_time;
        processes[i].initial_schedule_time = -1;
        processes[i].completion_time = -1;
        i++;
    }
    free(arr);
    int processes_num = i;
    
    //儲存的多個processes透過arrival_time進行透過arrival_time進行升冪排序
    // 呼叫排序（假設總共有 i 筆資料）
    qsort(processes, i, sizeof(Process), compare_arrival_time);

    //time用來記錄當前時間
    int time = 0;

    //queue用來記錄有哪些processes是ready的狀態
    char **queue = malloc(i * sizeof(char*));
    for (int k = 0; k < i; k++) {
        queue[k] = malloc(MAX_NAME);
    }

    int queue_index = -1;

    //running代表當前正在run的pid
    char running[100] = "";
    int running_index = 0;

    //定義一個指針用來指向下一個會被push進queue的processes
    int processes_ptr = 0;

    //用來判斷完程多少個processes 來判斷甚麼時候跳出while迴圈
    int finished = 0;
    printf("Running FCFS\nTime | Running | Ready Queue\n");
    printf("-----------------------------\n");
    while (finished < i) {
        if(time == 0 && processes[processes_ptr].arrival_time == 0) {
            strcpy(running, processes[processes_ptr++].pid);
            running_index = 0;               // 指向 P1
            if (processes[0].initial_schedule_time == -1)
                processes[0].initial_schedule_time = 0;
        }
        //如果cpu為空或是IDLE而且queue有ready的processes
        //則從queue pop出第一個ready的processes進去cpu執行
        //被pop進processes的就要記錄他的schedule_time
        if ((strcmp(running, "") == 0 || strcmp(running, "IDLE") == 0) && queue_index >= 0) {
            char* temp = pop(queue, &queue_index);
            strcpy(running, temp);
            free(temp);
            // 對應找到正確的 running_index
            for (int j = 0; j < processes_num; j++) {
                if (strcmp(processes[j].pid, running) == 0) {
                    running_index = j;
                    if (processes[j].initial_schedule_time == -1)
                        processes[j].initial_schedule_time = time;
                    break;
                }
            }
        }
        // 印出時間區間與正在執行的行程
        printf("%2d - %-2d | %-5s |", time, time + 1, strcmp(running,"")==0 ? "IDLE" : running);


        //如果下一秒是processes的arrival就push進queue
        if (processes_ptr < processes_num && time+1 == processes[processes_ptr].arrival_time) {
            queue_index = push(queue, queue_index, processes[processes_ptr++].pid);
        }
        // 印出 Ready Queue 中的行程
        if (queue_index < 0) {
            printf(" -\n");
        } 
        else {
            for (int q = 0; q <= queue_index; q++) {
                printf(" %s", queue[q]);
            }
            printf("\n");
        }
        
        //如果cpu有processes在執行 那processes的remain_time就要-1
        if (strcmp(running, "") != 0 && strcmp(running, "IDLE") != 0) {
            processes[running_index].remaining_time--;
            if(processes[running_index].remaining_time == 0) {
                finished++;
                processes[running_index].completion_time = time + 1;
                strcpy(running, ""); 
            }
        }

        time++;
    }
    double total_response = 0, total_turnaround = 0;
    for (int k = 0; k < i; k++) {
        total_response += processes[k].initial_schedule_time - processes[k].arrival_time;
        total_turnaround += processes[k].completion_time - processes[k].arrival_time;
    }
    printf("FCFS Execution Completed.\n");
    printf("Average Response Time: %.2f ms\n", total_response / i);
    printf("Average Turnaround Time: %.2f ms\n", total_turnaround / i);
    printf("Total Context Switches: %d\n", i - 1);
}

void run_rr(int argc, char *argv[]) {
    FILE *input;
    char filename[100];
    char buffer[MAX_BUFFER];
    sprintf(filename, "%s", argv[2]);
    input = fopen(filename, "r");

    //如果open失敗
    if (input == NULL) {
        printf("No such file or directory\n");
    }

    //創建arr存分割的子字串
    char** arr = malloc(3 * sizeof(char*));
    for (int j = 0; j < 3; j++) {
        arr[j] = malloc(MAX_NAME);
    }
    //process結構
    Process processes[1024];
    int i = 0;

    //讀取文字檔的每一行並做字串分割存到processes
    while (fgets(buffer, MAX_BUFFER, input) != NULL) {
        //printf("%s", buffer);
        int arr_index = 0;
        char *token;
        token = strtok(buffer, " ");
        while(token != NULL){
            arr[arr_index++] = token;
            token = strtok(NULL," ");
        }
        strcpy(processes[i].pid, arr[0]);
        processes[i].arrival_time = atoi(arr[1]);
        processes[i].run_time = atoi(arr[2]);
        processes[i].remaining_time = processes[i].run_time;
        processes[i].initial_schedule_time = -1;
        processes[i].completion_time = -1;
        i++;
    }
    free(arr);
    int processes_num = i;
    
    //儲存的多個processes透過arrival_time進行透過arrival_time進行升冪排序
    // 呼叫排序（假設總共有 i 筆資料）
    qsort(processes, i, sizeof(Process), compare_arrival_time);

    //time用來記錄當前時間
    int time = 0;

    //queue用來記錄有哪些processes是ready的狀態
    char **queue = malloc(i * sizeof(char*));
    for (int k = 0; k < i; k++) {
        queue[k] = malloc(MAX_NAME);
    }

    int queue_index = -1;

    //time_slice就是時間切片，每個processes一次可以運作的時間
    int time_slice = atoi(argv[3]);

    //running代表當前正在run的pid
    char running[100] = "";
    int running_index = 0;

    //定義一個指針用來指向下一個會被push進queue的processes
    int processes_ptr = 0;

    //用來判斷完程多少個processes 來判斷甚麼時候跳出while迴圈
    int finished = 0;

    int context_switches = 0;
    char last_running[MAX_NAME] = "IDLE";

    int exec_time;
    printf("Running RR\nTime | Running | Ready Queue\n");
    printf("-----------------------------\n");
    while (finished < i) {

        while (processes_ptr < processes_num && processes[processes_ptr].arrival_time <= time) {
            queue_index = push(queue, queue_index, processes[processes_ptr++].pid);
        }
        //如果當前時間cpu為空，而且也沒有任何ready的processes 那就直接time++
        if (strcmp(running, "") == 0 && queue_index < 0) {
            // 沒有 process 可跑，要 idle 一秒
            printf("%2d - %-2d | IDLE  | -\n", time, time + 1);
            time++;
        }

        //如果cpu為空，但是有ready的processes
        if ((strcmp(running, "") == 0 || strcmp(running, "IDLE") == 0) && queue_index >= 0) {
            char* temp = pop(queue, &queue_index);
            strcpy(running, temp);
            running_index = find_process_index_by_pid(processes, processes_num, running);
            if (processes[running_index].initial_schedule_time == -1)
                processes[running_index].initial_schedule_time = time;
            free(temp);
            if (strcmp(running, last_running) != 0 && strcmp(last_running, "IDLE") != 0) {
                context_switches++;
            }
            strcpy(last_running, running);
        }

        //如果cpu不為空
        if (strcmp(running, "") != 0 && strcmp(running, "IDLE") != 0) {

            //找到當前正在running的processes
            int idx = find_process_index_by_pid(processes, processes_num, running);
            if (processes[idx].initial_schedule_time == -1)
                processes[idx].initial_schedule_time = time;

            //紀錄實際執行的時間
            //狀況1.如果剩餘時間大於time_slice
            //狀況2.如果剩餘時間小於time_slice
            exec_time = processes[idx].remaining_time < time_slice ? processes[idx].remaining_time : time_slice;

            //判斷下一段時間是否有arrive的processes
            while (processes_ptr < processes_num && processes[processes_ptr].arrival_time <= time + exec_time) {
                queue_index = push(queue, queue_index, processes[processes_ptr++].pid);
            }

            // 印出時間區間與正在執行的行程
            printf("%2d - %-2d | %-5s |", time, time + exec_time, strcmp(running, "") == 0 ? "IDLE" : running);
            if (queue_index < 0) 
                printf(" -\n");
            else {
                for (int q = 0; q <= queue_index; q++) {
                    printf(" %s", queue[q]);
                }
                printf("\n");
            }

            time += exec_time;
            processes[idx].remaining_time -= exec_time;

            if (processes[idx].remaining_time == 0) {
                processes[idx].completion_time = time;
                finished++;
            }

            //當前running完但剩餘時間還沒歸零的processes要再push回去queue
            else {
                queue_index = push(queue, queue_index, running);
            }

            //清空cpu
            strcpy(running, "");
        }
    }
    
    // 計算 response 和 turnaround
    double total_response = 0, total_turnaround = 0;
    for (int k = 0; k < processes_num; k++) {
        total_response += processes[k].initial_schedule_time - processes[k].arrival_time;
        total_turnaround += processes[k].completion_time - processes[k].arrival_time;
    }
    printf("RR Execution Completed.\n");
    printf("Average Response Time: %.2f ms\n", total_response / processes_num);
    printf("Average Turnaround Time: %.2f ms\n", total_turnaround / processes_num);
    printf("Total Context Switches: %d\n", context_switches);
}

void usage(char *program_name) {
    printf("Usage: %s <fcfs|rr> <input_filename> [time_slice]\n", program_name);
    printf("  fcfs             Run First-Come-First-Serve scheduling\n");
    printf("  rr               Run Round-Robin scheduling\n");
    printf("  input_filename   Text file containing list of processes\n");
    printf("  time_slice       Required only for RR.\n");
    printf("\nExamples:\n");
    printf("  %s fcfs input1.txt\n", program_name);
    printf("  %s rr input2.txt 3\n", program_name);
}

int main(int argc, char *argv[]) {
    
        //如果是fcfs
        if (strcmp(argv[1],"fcfs") == 0) {
            run_fcfs(argc, argv);
        }
        //如果是rr
        else if (strcmp(argv[1],"rr") == 0) {
            run_rr(argc, argv);
        }
        //如果輸入錯誤
        else {
            printf("Error: Unknown scheduling policy '%s'.\n", argv[1]);
            usage(argv[0]);
            return 1;
        }
        return 0;
}