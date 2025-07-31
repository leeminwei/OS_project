#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

/* 緩衝區相關常數 */
#define BUFFER_SIZE 8        // 緩衝區大小
#define MAX_ITEMS 30         // 生產者要產生的項目總數

/* 延遲時間（微秒） */
#define PRODUCER_DELAY 100000    // 生產者延遲 (0.1秒)
#define CONSUMER_DELAY 150000    // 消費者延遲 (0.15秒)

/* 緩衝區結構 */
typedef struct {
    int buffer[BUFFER_SIZE];    // 存放數據的陣列
    int in;                     // 下一個寫入位置
    int out;                    // 下一個讀取位置
    int count;                  // 當前緩衝區中的項目數量
} circular_buffer_t;

/* 共用函數聲明 */
void print_timestamp(void);
void print_buffer_status(circular_buffer_t *buf, const char *operation, int item);
long get_timestamp_ms(void);

#endif /* COMMON_H */ 