/**
 * 生產者消費者問題 - 使用系統信號量版本
 * 
 * 此程式展示如何使用POSIX系統信號量來解決經典的生產者消費者問題
 * 使用有界緩衝區來協調生產者和消費者之間的同步
 */

#include "common.h"
#include <semaphore.h>
#include <fcntl.h>

/* 全域變數 */
circular_buffer_t shared_buffer;    // 共享緩衝區
sem_t empty_slots;                 // 空閒槽位信號量
sem_t full_slots;                  // 已用槽位信號量
pthread_mutex_t buffer_mutex;       // 緩衝區互斥鎖


int produced_items = 0;             // 已生產的項目數
int consumed_items = 0;             // 已消費的項目數

/**
 * 初始化緩衝區
 */
void init_buffer(circular_buffer_t *buf) {
    // TODO
    buf->count = 0;
    buf->in = 0;
    buf->out = 0;
}

/**
 * 向緩衝區添加項目
 */
void buffer_put(circular_buffer_t *buf, int item) {
    // TODO
    //如果buffer是滿的
    if (buf->count == 8) {
        printf("buffer已滿\n");
        return;
    }
    buf->buffer[buf->in] = item;
    buf->in = (buf->in + 1) % BUFFER_SIZE;
    buf->count++;
    print_buffer_status(buf, "放入", item);
    return;
}

/**
 * 從緩衝區取出項目
 */
int buffer_get(circular_buffer_t *buf) {
    // TODO
    //如果buffer是空的
    if (buf->count == 0) {
        printf("buffer為空\n");
        return -1;
    }
    int value = buf->buffer[buf->out];
    buf->out = (buf->out + 1) % BUFFER_SIZE;
    buf->count--;
    print_buffer_status(buf, "取出", value);
    return value;
}

/**
 * 生產者執行緒函數
 */
void* producer(void* arg) {
    //TODO
    for (int i = 0; i < MAX_ITEMS; i++) {
        sem_wait(&empty_slots);                   // 等待有空位
        pthread_mutex_lock(&buffer_mutex);
        buffer_put(&shared_buffer, i + 1);        // 放入編號為 i+1 的資料
        produced_items++;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full_slots);                    // 通知有新資料可以消費

        usleep(PRODUCER_DELAY); // 模擬延遲（100ms，可選）
    }
    return NULL;
}

/**
 * 消費者執行緒函數
 */
void* consumer(void* arg) {
    //TODO
    for (int i = 0; i < MAX_ITEMS; i++) {
        sem_wait(&full_slots);
        pthread_mutex_lock(&buffer_mutex);
        int value = buffer_get(&shared_buffer);
        consumed_items++;
        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty_slots);
        usleep(CONSUMER_DELAY);
    }
    return NULL;
}

/**
 * 主函數
 */
int main() {
    printf("=== 生產者消費者問題 - 系統信號量版本 ===\n");
    printf("緩衝區大小: %d\n", BUFFER_SIZE);
    printf("項目總數: %d\n", MAX_ITEMS);
    printf("生產者延遲: %.1f秒\n", PRODUCER_DELAY / 1000000.0);
    printf("消費者延遲: %.1f秒\n", CONSUMER_DELAY / 1000000.0);
    printf("==========================================\n\n");
    
    long start_time = get_timestamp_ms();
    
    // 初始化緩衝區
    init_buffer(&shared_buffer);
    
    // 初始化系統信號量
    if (sem_init(&empty_slots, 0, BUFFER_SIZE) != 0) {
        perror("❌ 初始化empty_slots信號量失敗");
        exit(1);
    }
    
    if (sem_init(&full_slots, 0, 0) != 0) {
        perror("❌ 初始化full_slots信號量失敗");
        sem_destroy(&empty_slots);
        exit(1);
    }
    
    // 初始化互斥鎖
    if (pthread_mutex_init(&buffer_mutex, NULL) != 0) {
        perror("❌ 初始化緩衝區互斥鎖失敗");
        sem_destroy(&empty_slots);
        sem_destroy(&full_slots);
        exit(1);
    }
    
    // 創建執行緒
    pthread_t producer_thread, consumer_thread;
    
    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0) {
        perror("❌ 創建生產者執行緒失敗");
        exit(1);
    }
    
    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        perror("❌ 創建消費者執行緒失敗");
        exit(1);
    }
    
    // 等待執行緒結束
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    long end_time = get_timestamp_ms();
    
    // 清理資源
    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&buffer_mutex);
    
    printf("\n==========================================\n");
    printf("✅ 程式執行完成！\n");
    printf("總執行時間: %.2f 秒\n", (end_time - start_time) / 1000.0);
    printf("生產項目: %d | 消費項目: %d\n", produced_items, consumed_items);
    printf("==========================================\n");
    
    return 0;
} 