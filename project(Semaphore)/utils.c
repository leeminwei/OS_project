#include "common.h"

/**
 * 列印當前時間戳
 */
void print_timestamp(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    struct tm *tm_info = localtime(&tv.tv_sec);
    printf("[%02d:%02d:%02d.%03d] ", 
           tm_info->tm_hour, 
           tm_info->tm_min, 
           tm_info->tm_sec, 
           (int)(tv.tv_usec / 1000));
}

/**
 * 獲取時間戳（毫秒）
 */
long get_timestamp_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

/**
 * 列印緩衝區狀態
 */
void print_buffer_status(circular_buffer_t *buf, const char *operation, int item) {
    print_timestamp();
    printf("%s項目 %d | 緩衝區狀態: [", operation, item);
    
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (i < buf->count) {
            int index = (buf->out + i) % BUFFER_SIZE;
            printf("%2d", buf->buffer[index]);
        } else {
            printf(" -");
        }
        if (i < BUFFER_SIZE - 1) printf(",");
    }
    
    printf("] (數量: %d/%d)\n", buf->count, BUFFER_SIZE);
} 