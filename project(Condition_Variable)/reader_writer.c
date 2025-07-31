#include "reader_writer.h"

/* ========== 讀者-寫者鎖實作 ========== */

int rw_lock_init(rw_lock_t *rw_lock) {
    //TODO
    if (rw_lock == NULL) return -1;

    if (pthread_mutex_init(&rw_lock->mutex, NULL) != 0) return -1;
    if (pthread_cond_init(&rw_lock->read_cond, NULL) != 0) return -1;
    if (pthread_cond_init(&rw_lock->write_cond, NULL) != 0) return -1;

    rw_lock->readers_count = 0;
    rw_lock->writers_count = 0;
    rw_lock->waiting_writers = 0;
    rw_lock->total_reads = 0;
    rw_lock->total_writes = 0;

    return 0;
}

int rw_lock_destroy(rw_lock_t *rw_lock) {
    //TODO
    if (rw_lock == NULL) return -1;
    if (pthread_mutex_destroy(&rw_lock->mutex) != 0) return -1;
    if (pthread_cond_destroy(&rw_lock->read_cond) != 0) return -1;
    if (pthread_cond_destroy(&rw_lock->write_cond) != 0) return -1;

    return 0;
}

int rw_lock_rdlock(rw_lock_t *rw_lock) {
    //TODO
    if (rw_lock == NULL) return -1;
    pthread_mutex_lock(&rw_lock->mutex);

    //如果有寫者在寫，或有寫者在排隊 → 讀者就得等
    while (rw_lock->waiting_writers > 0 || rw_lock->writers_count > 0) {
        pthread_cond_wait(&rw_lock->read_cond, &rw_lock->mutex);
    }

    //如果沒有寫者
    rw_lock->readers_count++;
    rw_lock->total_reads++;
    pthread_mutex_unlock(&rw_lock->mutex);
    return 0;
}

int rw_lock_rdunlock(rw_lock_t *rw_lock) {
    //TODO
    if (rw_lock == NULL) return -1;
    pthread_mutex_lock(&rw_lock->mutex);
    rw_lock->readers_count--;
    // 如果沒有讀者了 → 喚醒等待中的寫者
    if (rw_lock->readers_count == 0) {
        pthread_cond_signal(&rw_lock->write_cond);
    }
    pthread_mutex_unlock(&rw_lock->mutex);
    return 0;
}

int rw_lock_wrlock(rw_lock_t *rw_lock) {
    //TODO
    if (rw_lock == NULL) return -1;
    pthread_mutex_lock(&rw_lock->mutex);
    rw_lock->waiting_writers++;
    // 等到沒有讀者也沒有寫者時，才能進入
    while (rw_lock->readers_count > 0 || rw_lock->writers_count > 0) {
        pthread_cond_wait(&rw_lock->write_cond, &rw_lock->mutex);
    }
    rw_lock->waiting_writers--;
    rw_lock->total_writes++;
    rw_lock->writers_count++;
    pthread_mutex_unlock(&rw_lock->mutex);
    return 0;
}

int rw_lock_wrunlock(rw_lock_t *rw_lock) {
    //TODO
    if (rw_lock == NULL) return -1;
    pthread_mutex_lock(&rw_lock->mutex);
    rw_lock->writers_count--;
    //如果有其他寫者
    if (rw_lock->waiting_writers > 0) {
        pthread_cond_signal(&rw_lock->write_cond);
    }
    //如果沒有寫者
    else{
        pthread_cond_signal(&rw_lock->read_cond);
    }
    pthread_mutex_unlock(&rw_lock->mutex);
    return 0;
}

/* ========== 資料庫操作實作 ========== */

void database_init(shared_database_t *db) {
    //TODO
    if (db == NULL) return;
    strcpy(db->data, "");
    db->data_version = 1;
    db->last_modified = time(NULL);
}

int database_read(shared_database_t *db, char *buffer, size_t size) {
    //TODO
    if (db == NULL || buffer == NULL) return -1;
    if (size == 0) return -1;
    // 複製最多 size - 1 個字元，確保最後一格保留給 '\0'
    strncpy(buffer, db->data, size - 1);
    buffer[size - 1] = '\0';  // 確保字串結尾
    return strlen(buffer); 
}

int database_write(shared_database_t *db, const char *data) {
    //TODO
    if (db == NULL || data == NULL) return -1;
    if (strlen(data) >= sizeof(db->data)) return -1;
    strcpy(db->data, data);
    db->data_version++;
    db->last_modified = time(NULL);
    return 0;
}

/* ========== 執行緒函數實作 ========== */

void *reader_thread(void *arg) {
    //TODO
    thread_args_t *args = (thread_args_t *)arg;
    //printf("ID = %d\n", args->thread_id);

    //buffer用來存取讀取資料庫的字串內容
    //1024代表1024個字元, 一個字元至少 1 bytes, 所以至少占了 1024 bytes, 也就是10kB
    char* buffer = (char*)malloc(1024 * sizeof(char));
    size_t size = 1024;

    //每個執行續執行operation次
    for (int i=0 ; i<args->operations ; i++) {
        rw_lock_rdlock(args->rw_lock);
        database_read(args->database, buffer, size);
        printf("Reader ID = %d read: %s\n", args->thread_id, buffer);
        rw_lock_rdunlock(args->rw_lock);
        usleep(args->sleep_ms * 1000);
    }
    free(buffer);
    return NULL;
}

void *writer_thread(void *arg) {
    //TODO
    thread_args_t *args = (thread_args_t *)arg;
    char buffer[128]; 
    for (int i=0 ; i<args->operations ; i++) {
        rw_lock_wrlock(args->rw_lock);
        snprintf(buffer, sizeof(buffer), "寫者 %d 第 %d 次寫入：模擬資料內容", args->thread_id, i + 1);
        database_write(args->database, buffer);
        printf("Writer %d wrote: %s\n", args->thread_id, buffer);
        rw_lock_wrunlock(args->rw_lock);
        usleep(args->sleep_ms * 1000);
    }
    return NULL;
}

/* ========== 工具函數實作 ========== */

void print_rw_lock_status(rw_lock_t *rw_lock) {
    //TODO
    if (rw_lock == NULL) return;
    pthread_mutex_lock(&rw_lock->mutex);
    printf("目前讀者數        : %d\n", rw_lock->readers_count);
    printf("目前寫者數        : %d\n", rw_lock->writers_count);
    printf("等待中的寫者數    : %d\n", rw_lock->waiting_writers);
    printf("累積讀取次數      : %d\n", rw_lock->total_reads);
    printf("累積寫入次數      : %d\n", rw_lock->total_writes);
    pthread_mutex_unlock(&rw_lock->mutex);
}

void run_reader_writer_test(int num_readers, int num_writers, 
                           int operations_per_thread,
                           int reader_sleep_ms, int writer_sleep_ms) {
    //TODO

    //初始化資料庫
    shared_database_t *db = (shared_database_t*)malloc(sizeof(shared_database_t));
    database_init(db);

    //初始化鎖
    rw_lock_t* lock = (rw_lock_t*)malloc(sizeof(rw_lock_t));
    rw_lock_init(lock);

    thread_args_t args[num_readers + num_writers];
    pthread_t tid[num_readers + num_writers];

    for (int i=0 ; i<num_readers ; i++) {
        args[i].thread_id = i;
        args[i].database = db;
        args[i].operations = operations_per_thread;
        args[i].rw_lock = lock;
        args[i].sleep_ms = reader_sleep_ms;
        pthread_create(&tid[i], NULL, reader_thread, &args[i]);
    }
    for (int i=num_readers ; i< num_readers + num_writers ; i++) {
        args[i].thread_id = i-num_readers;
        args[i].database = db;
        args[i].operations = operations_per_thread;
        args[i].rw_lock = lock;
        args[i].sleep_ms = writer_sleep_ms;
        pthread_create(&tid[i], NULL, writer_thread, &args[i]);
    }
    //等待所有執行續完成
    for (int i=0 ; i<(num_readers+num_writers) ; i++) {
        pthread_join(tid[i], NULL);
    }
    print_rw_lock_status(lock);
    rw_lock_destroy(lock);
    free(db);
}

/* ========== 主程式 ========== */

#ifndef LIB_BUILD
int main(int argc, char *argv[]) {
    //TODO
    int num_readers = 3;
    int num_writers = 2;
    int operations = 3;
    int reader_sleep = 100;  // 100ms
    int writer_sleep = 200;  // 200ms
    
    // 解析命令列參數
    if (argc >= 6) {
        num_readers = atoi(argv[1]);
        num_writers = atoi(argv[2]);
        operations = atoi(argv[3]);
        reader_sleep = atoi(argv[4]);
        writer_sleep = atoi(argv[5]);
    } else if (argc > 1) {
        printf("使用方法: %s [讀者數量] [寫者數量] [操作次數] [讀者延遲ms] [寫者延遲ms]\n", argv[0]);
        printf("使用預設參數進行測試...\n\n");
    }
    
    // 驗證參數
    if (num_readers < 0 || num_writers < 0 || operations < 1) {
        fprintf(stderr, "參數錯誤: 讀者和寫者數量必須 >= 0，操作次數必須 >= 1\n");
        return 1;
    }
    
    printf("=== 讀者-寫者問題示範程式 ===\n");
    printf("基於條件變數的同步機制實作\n\n");
    
    // 執行測試
    run_reader_writer_test(num_readers, num_writers, operations, reader_sleep, writer_sleep);
    
    printf("\n測試完成！\n");
    return 0;
}
#endif /* LIB_BUILD */ 