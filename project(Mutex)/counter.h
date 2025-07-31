#ifndef COUNTER_H
#define COUNTER_H

#include <pthread.h>

/* 執行緒安全計數器 */
typedef struct {
    int value;
    pthread_mutex_t lock;
} counter_t;

/* 初始化計數器，設定初始值並初始化 mutex */
void counter_init(counter_t *c, int init_value);

/* 銷毀計數器所使用的資源 */
void counter_destroy(counter_t *c);

/* 將計數器加 amount (可為負) */
void counter_increment(counter_t *c, int amount);

/* 取得目前計數器的值 */
int counter_get(counter_t *c);

#endif /* COUNTER_H */ 