#ifndef CUSTOM_SEMAPHORE_H
#define CUSTOM_SEMAPHORE_H

#include <pthread.h>

/* 自製信號量結構 */
typedef struct {
    int value;                  // 信號量的值
    pthread_mutex_t mutex;      // 保護信號量的互斥鎖
    pthread_cond_t condition;   // 條件變數
} custom_sem_t;

/* 自製信號量函數聲明 */

/**
 * 初始化自製信號量
 * @param sem 信號量指針
 * @param value 初始值
 * @return 成功返回0，失敗返回-1
 */
int custom_sem_init(custom_sem_t *sem, int value);

/**
 * 銷毀自製信號量
 * @param sem 信號量指針
 * @return 成功返回0，失敗返回-1
 */
int custom_sem_destroy(custom_sem_t *sem);

/**
 * P操作（等待操作）- 減少信號量值
 * 如果值為0則阻塞，直到值大於0
 * @param sem 信號量指針
 */
void custom_sem_wait(custom_sem_t *sem);

/**
 * V操作（信號操作）- 增加信號量值
 * 喚醒一個等待的執行緒
 * @param sem 信號量指針
 */
void custom_sem_post(custom_sem_t *sem);

/**
 * 獲取信號量當前值（僅用於調試）
 * @param sem 信號量指針
 * @return 當前值
 */
int custom_sem_getvalue(custom_sem_t *sem);

#endif /* CUSTOM_SEMAPHORE_H */ 