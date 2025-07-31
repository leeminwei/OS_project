#include "custom_semaphore.h"
#include <stdio.h>
#include <errno.h>

/**
 * 初始化自製信號量
 */
int custom_sem_init(custom_sem_t *sem, int value) { 
    // TODO 
    sem->value = value;
    if (pthread_mutex_init(&sem->mutex, NULL) != 0) return -1;
    if (pthread_cond_init(&sem->condition, NULL) != 0) return -1;
    return 0;
}

/**
 * 銷毀自製信號量
 */
int custom_sem_destroy(custom_sem_t *sem) {  
    // TODO
    if (pthread_mutex_destroy(&sem->mutex) != 0) return -1;
    if (pthread_cond_destroy(&sem->condition) != 0) return -1;
    return 0;
}

/**
 * P操作（等待操作）- 減少信號量值
 * 相當於 sem_wait()
 */
void custom_sem_wait(custom_sem_t *sem) {
    // TODO
    pthread_mutex_lock(&sem->mutex);
    sem->value--;
    while (sem->value < 0) {
        pthread_cond_wait(&sem->condition, &sem->mutex);
    }
    pthread_mutex_unlock(&sem->mutex);
    return;
}

/**
 * V操作（信號操作）- 增加信號量值  
 * 相當於 sem_post()
 */
void custom_sem_post(custom_sem_t *sem) {
    // TODO
    pthread_mutex_lock(&sem->mutex);
    sem->value++;
    //如果用while就會造成死迴圈
    if (sem->value >= 0) {
        pthread_cond_signal(&sem->condition);
    }
    pthread_mutex_unlock(&sem->mutex);
}

/**
 * 獲取信號量當前值（僅用於調試）
 */
int custom_sem_getvalue(custom_sem_t *sem) {
    // TODO
    pthread_mutex_lock(&sem->mutex);
    int val = sem->value;
    pthread_mutex_unlock(&sem->mutex);
    return val;
} 