#include "counter.h"
#include <stdlib.h>

/* TODO: 在以下函式中加入適當的 mutex 操作，使其在多執行緒環境下仍能正確運作 */

void counter_init(counter_t *c, int init_value) {
    /* TODO */
    c->value = init_value;
    pthread_mutex_init(&c->lock, NULL);
}

void counter_destroy(counter_t *c) {
    /* TODO */
    pthread_mutex_destroy(&c->lock);
}

void counter_increment(counter_t *c, int amount) {
    /* TODO */
    pthread_mutex_lock(&c->lock);
    c->value += amount;
    pthread_mutex_unlock(&c->lock); 
}

int counter_get(counter_t *c) {
    /* TODO */
    return c->value;
} 