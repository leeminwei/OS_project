#ifndef READER_WRITER_H
#define READER_WRITER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

/**
 * 共享資料庫結構
 * 模擬一個可被多個讀者同時讀取，但只能被一個寫者獨占存取的資料庫
 */
typedef struct {
    char data[1024];           // 共享資料
    int data_version;          // 資料版本號
    time_t last_modified;      // 最後修改時間
} shared_database_t;

/**
 * 讀者-寫者鎖結構
 * 包含所有必要的同步變數
 */
typedef struct {
    pthread_mutex_t mutex;          // 保護共享變數的互斥鎖
    pthread_cond_t read_cond;       // 讀者條件變數
    pthread_cond_t write_cond;      // 寫者條件變數
    
    int readers_count;              // 目前讀者數量
    int writers_count;              // 目前寫者數量（0或1）
    int waiting_writers;            // 等待中的寫者數量
    
    // 統計資料
    int total_reads;                // 總讀取次數
    int total_writes;               // 總寫入次數
} rw_lock_t;

/**
 * 執行緒參數結構
 */
typedef struct {
    int thread_id;                  // 執行緒ID
    int operations;                 // 要執行的操作次數
    rw_lock_t *rw_lock;            // 讀者-寫者鎖
    shared_database_t *database;   // 共享資料庫
    int sleep_ms;                   // 模擬操作時間（毫秒）
} thread_args_t;

/* 讀者-寫者鎖函數 */

/**
 * 初始化讀者-寫者鎖
 * @param rw_lock 指向讀者-寫者鎖結構的指標
 * @return 成功返回0，失敗返回-1
 */
int rw_lock_init(rw_lock_t *rw_lock);

/**
 * 銷毀讀者-寫者鎖
 * @param rw_lock 指向讀者-寫者鎖結構的指標
 * @return 成功返回0，失敗返回-1
 */
int rw_lock_destroy(rw_lock_t *rw_lock);

/**
 * 獲取讀者鎖
 * @param rw_lock 指向讀者-寫者鎖結構的指標
 * @return 成功返回0，失敗返回-1
 */
int rw_lock_rdlock(rw_lock_t *rw_lock);

/**
 * 釋放讀者鎖
 * @param rw_lock 指向讀者-寫者鎖結構的指標
 * @return 成功返回0，失敗返回-1
 */
int rw_lock_rdunlock(rw_lock_t *rw_lock);

/**
 * 獲取寫者鎖
 * @param rw_lock 指向讀者-寫者鎖結構的指標
 * @return 成功返回0，失敗返回-1
 */
int rw_lock_wrlock(rw_lock_t *rw_lock);

/**
 * 釋放寫者鎖
 * @param rw_lock 指向讀者-寫者鎖結構的指標
 * @return 成功返回0，失敗返回-1
 */
int rw_lock_wrunlock(rw_lock_t *rw_lock);

/* 資料庫操作函數 */

/**
 * 初始化共享資料庫
 * @param db 指向資料庫結構的指標
 */
void database_init(shared_database_t *db);

/**
 * 讀取資料庫內容
 * @param db 指向資料庫結構的指標
 * @param buffer 存放讀取資料的緩衝區
 * @param size 緩衝區大小
 * @return 讀取的字節數
 */
int database_read(shared_database_t *db, char *buffer, size_t size);

/**
 * 寫入資料庫內容
 * @param db 指向資料庫結構的指標
 * @param data 要寫入的資料
 * @return 成功返回0，失敗返回-1
 */
int database_write(shared_database_t *db, const char *data);

/* 執行緒函數 */

/**
 * 讀者執行緒函數
 * @param arg 指向thread_args_t結構的指標
 * @return NULL
 */
void *reader_thread(void *arg);

/**
 * 寫者執行緒函數
 * @param arg 指向thread_args_t結構的指標
 * @return NULL
 */
void *writer_thread(void *arg);

/* 工具函數 */

/**
 * 列印讀者-寫者鎖的狀態
 * @param rw_lock 指向讀者-寫者鎖結構的指標
 */
void print_rw_lock_status(rw_lock_t *rw_lock);

/**
 * 執行讀者-寫者測試
 * @param num_readers 讀者數量
 * @param num_writers 寫者數量
 * @param operations_per_thread 每個執行緒的操作次數
 * @param reader_sleep_ms 讀者操作時間（毫秒）
 * @param writer_sleep_ms 寫者操作時間（毫秒）
 */
void run_reader_writer_test(int num_readers, int num_writers, 
                           int operations_per_thread,
                           int reader_sleep_ms, int writer_sleep_ms);

#endif /* READER_WRITER_H */ 