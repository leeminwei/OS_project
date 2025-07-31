#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "../counter.h"

#define THREADS 4
#define LOOPS 1000000

void* worker(void* arg) {
    counter_t* c = (counter_t*)arg;
    for (int i = 0; i < LOOPS; ++i) {
        counter_increment(c, 1);
    }
    return NULL;
}

int main(void) {
    counter_t counter;
    counter_init(&counter, 0);

    pthread_t tid[THREADS];

    for (int i = 0; i < THREADS; ++i) {
        pthread_create(&tid[i], NULL, worker, &counter);
    }

    for (int i = 0; i < THREADS; ++i) {
        pthread_join(tid[i], NULL);
    }

    int expected = THREADS * LOOPS;
    int result   = counter_get(&counter);
    printf("Expected %d, got %d\n", expected, result);
    assert(result == expected && "計數器結果不正確，請檢查 mutex 使用");
    counter_destroy(&counter);
    printf("Test passed!\n");
    return 0;
} 