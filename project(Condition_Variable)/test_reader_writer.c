#include "reader_writer.h"
#include <assert.h>

/* ========== 測試輔助函數 ========== */

static int test_passed = 0;
static int test_failed = 0;

#define TEST_ASSERT(condition, test_name) \
    do { \
        if (condition) { \
            printf("✓ %s: 通過\n", test_name); \
            test_passed++; \
        } else { \
            printf("✗ %s: 失敗\n", test_name); \
            test_failed++; \
        } \
    } while (0)

void print_test_results() {
    printf("\n=== 測試結果摘要 ===\n");
    printf("通過: %d\n", test_passed);
    printf("失敗: %d\n", test_failed);
    printf("總計: %d\n", test_passed + test_failed);
    if (test_failed == 0) {
        printf("🎉 所有測試都通過了！\n");
    } else {
        printf("⚠️  有 %d 個測試失敗\n", test_failed);
    }
    printf("==================\n\n");
}

/* ========== 基本功能測試 ========== */

void test_rw_lock_init_destroy() {
    printf("\n--- 測試: 讀者-寫者鎖初始化與銷毀 ---\n");
    
    rw_lock_t lock;
    
    // 測試初始化
    int init_result = rw_lock_init(&lock);
    TEST_ASSERT(init_result == 0, "讀者-寫者鎖初始化");
    
    // 測試初始狀態
    pthread_mutex_lock(&lock.mutex);
    TEST_ASSERT(lock.readers_count == 0, "初始讀者計數為0");
    TEST_ASSERT(lock.writers_count == 0, "初始寫者計數為0");
    TEST_ASSERT(lock.waiting_writers == 0, "初始等待寫者計數為0");
    TEST_ASSERT(lock.total_reads == 0, "初始總讀取次數為0");
    TEST_ASSERT(lock.total_writes == 0, "初始總寫入次數為0");
    pthread_mutex_unlock(&lock.mutex);
    
    // 測試銷毀
    int destroy_result = rw_lock_destroy(&lock);
    TEST_ASSERT(destroy_result == 0, "讀者-寫者鎖銷毀");
    
    // 測試NULL指標處理
    TEST_ASSERT(rw_lock_init(NULL) == -1, "NULL指標初始化處理");
    TEST_ASSERT(rw_lock_destroy(NULL) == -1, "NULL指標銷毀處理");
}

void test_database_operations() {
    printf("\n--- 測試: 資料庫操作 ---\n");
    
    shared_database_t db;
    char buffer[1024];
    
    // 測試初始化
    database_init(&db);
    TEST_ASSERT(strlen(db.data) > 0, "資料庫初始化後有內容");
    TEST_ASSERT(db.data_version == 1, "初始版本號為1");
    
    // 測試讀取
    int bytes_read = database_read(&db, buffer, sizeof(buffer));
    TEST_ASSERT(bytes_read > 0, "資料庫讀取成功");
    TEST_ASSERT(strcmp(buffer, db.data) == 0, "讀取內容正確");
    
    // 測試寫入
    const char *test_data = "這是測試資料";
    int write_result = database_write(&db, test_data);
    TEST_ASSERT(write_result == 0, "資料庫寫入成功");
    TEST_ASSERT(strcmp(db.data, test_data) == 0, "寫入內容正確");
    TEST_ASSERT(db.data_version == 2, "版本號正確遞增");
    
    // 測試錯誤處理
    TEST_ASSERT(database_read(NULL, buffer, sizeof(buffer)) == -1, "NULL資料庫讀取錯誤處理");
    TEST_ASSERT(database_read(&db, NULL, sizeof(buffer)) == -1, "NULL緩衝區讀取錯誤處理");
    TEST_ASSERT(database_write(NULL, test_data) == -1, "NULL資料庫寫入錯誤處理");
    TEST_ASSERT(database_write(&db, NULL) == -1, "NULL資料寫入錯誤處理");
}

void test_basic_rw_lock_operations() {
    printf("\n--- 測試: 基本讀者-寫者鎖操作 ---\n");
    
    rw_lock_t lock;
    rw_lock_init(&lock);
    
    // 測試讀者鎖
    int rd_lock_result = rw_lock_rdlock(&lock);
    TEST_ASSERT(rd_lock_result == 0, "獲取讀者鎖");
    
    pthread_mutex_lock(&lock.mutex);
    TEST_ASSERT(lock.readers_count == 1, "讀者計數正確");
    TEST_ASSERT(lock.total_reads == 1, "總讀取次數正確");
    pthread_mutex_unlock(&lock.mutex);
    
    int rd_unlock_result = rw_lock_rdunlock(&lock);
    TEST_ASSERT(rd_unlock_result == 0, "釋放讀者鎖");
    
    pthread_mutex_lock(&lock.mutex);
    TEST_ASSERT(lock.readers_count == 0, "讀者計數歸零");
    pthread_mutex_unlock(&lock.mutex);
    
    // 測試寫者鎖
    int wr_lock_result = rw_lock_wrlock(&lock);
    TEST_ASSERT(wr_lock_result == 0, "獲取寫者鎖");
    
    pthread_mutex_lock(&lock.mutex);
    TEST_ASSERT(lock.writers_count == 1, "寫者計數正確");
    TEST_ASSERT(lock.total_writes == 1, "總寫入次數正確");
    pthread_mutex_unlock(&lock.mutex);
    
    int wr_unlock_result = rw_lock_wrunlock(&lock);
    TEST_ASSERT(wr_unlock_result == 0, "釋放寫者鎖");
    
    pthread_mutex_lock(&lock.mutex);
    TEST_ASSERT(lock.writers_count == 0, "寫者計數歸零");
    pthread_mutex_unlock(&lock.mutex);
    
    // 測試錯誤處理
    TEST_ASSERT(rw_lock_rdlock(NULL) == -1, "NULL指標讀者鎖錯誤處理");
    TEST_ASSERT(rw_lock_rdunlock(NULL) == -1, "NULL指標讀者解鎖錯誤處理");
    TEST_ASSERT(rw_lock_wrlock(NULL) == -1, "NULL指標寫者鎖錯誤處理");
    TEST_ASSERT(rw_lock_wrunlock(NULL) == -1, "NULL指標寫者解鎖錯誤處理");
    
    rw_lock_destroy(&lock);
}

/* ========== 同步測試 ========== */

typedef struct {
    rw_lock_t *lock;
    shared_database_t *db;
    int *counter;
    int thread_id;
    int delay_ms;
} sync_test_args_t;

void *multiple_readers_test_thread(void *arg) {
    sync_test_args_t *args = (sync_test_args_t *)arg;
    char buffer[1024];
    
    rw_lock_rdlock(args->lock);
    
    // 記錄開始時間和當前讀者數量
    pthread_mutex_lock(&args->lock->mutex);
    int current_readers = args->lock->readers_count;
    (*args->counter)++;
    pthread_mutex_unlock(&args->lock->mutex);
    
    // 模擬讀取操作
    database_read(args->db, buffer, sizeof(buffer));
    
    if (args->delay_ms > 0) {
        usleep(args->delay_ms * 1000);
    }
    
    rw_lock_rdunlock(args->lock);
    
    return (void *)(intptr_t)current_readers;
}

void test_multiple_readers() {
    printf("\n--- 測試: 多個讀者同時存取 ---\n");
    
    const int num_readers = 5;
    rw_lock_t lock;
    shared_database_t db;
    int access_counter = 0;
    
    rw_lock_init(&lock);
    database_init(&db);
    
    pthread_t readers[num_readers];
    sync_test_args_t args[num_readers];
    
    // 啟動多個讀者執行緒
    for (int i = 0; i < num_readers; i++) {
        args[i].lock = &lock;
        args[i].db = &db;
        args[i].counter = &access_counter;
        args[i].thread_id = i;
        args[i].delay_ms = 100; // 100ms延遲
        
        pthread_create(&readers[i], NULL, multiple_readers_test_thread, &args[i]);
    }
    
    // 等待所有讀者完成
    int max_concurrent_readers = 0;
    for (int i = 0; i < num_readers; i++) {
        void *result;
        pthread_join(readers[i], &result);
        int concurrent_readers = (int)(intptr_t)result;
        if (concurrent_readers > max_concurrent_readers) {
            max_concurrent_readers = concurrent_readers;
        }
    }
    
    TEST_ASSERT(access_counter == num_readers, "所有讀者都成功存取");
    TEST_ASSERT(max_concurrent_readers > 1, "多個讀者同時存取");
    
    pthread_mutex_lock(&lock.mutex);
    TEST_ASSERT(lock.readers_count == 0, "所有讀者都已釋放鎖");
    TEST_ASSERT(lock.total_reads == num_readers, "總讀取次數正確");
    pthread_mutex_unlock(&lock.mutex);
    
    rw_lock_destroy(&lock);
}

void *writer_exclusion_test_thread(void *arg) {
    sync_test_args_t *args = (sync_test_args_t *)arg;
    char write_data[128];
    
    snprintf(write_data, sizeof(write_data), "寫者 %d 的資料", args->thread_id);
    
    rw_lock_wrlock(args->lock);
    
    // 檢查是否有其他寫者或讀者
    pthread_mutex_lock(&args->lock->mutex);
    int readers = args->lock->readers_count;
    int writers = args->lock->writers_count;
    (*args->counter)++;
    pthread_mutex_unlock(&args->lock->mutex);
    
    // 模擬寫入操作
    database_write(args->db, write_data);
    
    if (args->delay_ms > 0) {
        usleep(args->delay_ms * 1000);
    }
    
    rw_lock_wrunlock(args->lock);
    
    // 返回當時的讀者和寫者數量（應該都是0，除了自己）
    return (void *)(intptr_t)((readers << 16) | (writers - 1));
}

void test_writer_exclusion() {
    printf("\n--- 測試: 寫者獨占存取 ---\n");
    
    const int num_writers = 3;
    rw_lock_t lock;
    shared_database_t db;
    int write_counter = 0;
    
    rw_lock_init(&lock);
    database_init(&db);
    
    pthread_t writers[num_writers];
    sync_test_args_t args[num_writers];
    
    // 啟動多個寫者執行緒
    for (int i = 0; i < num_writers; i++) {
        args[i].lock = &lock;
        args[i].db = &db;
        args[i].counter = &write_counter;
        args[i].thread_id = i;
        args[i].delay_ms = 50; // 50ms延遲
        
        pthread_create(&writers[i], NULL, writer_exclusion_test_thread, &args[i]);
    }
    
    // 等待所有寫者完成
    int exclusion_violations = 0;
    for (int i = 0; i < num_writers; i++) {
        void *result;
        pthread_join(writers[i], &result);
        int combined = (int)(intptr_t)result;
        int readers = (combined >> 16) & 0xFFFF;
        int other_writers = combined & 0xFFFF;
        
        if (readers > 0 || other_writers > 0) {
            exclusion_violations++;
        }
    }
    
    TEST_ASSERT(write_counter == num_writers, "所有寫者都成功存取");
    TEST_ASSERT(exclusion_violations == 0, "寫者獨占存取正確");
    
    pthread_mutex_lock(&lock.mutex);
    TEST_ASSERT(lock.writers_count == 0, "所有寫者都已釋放鎖");
    TEST_ASSERT(lock.total_writes == num_writers, "總寫入次數正確");
    pthread_mutex_unlock(&lock.mutex);
    
    rw_lock_destroy(&lock);
}

/* ========== 壓力測試 ========== */

void test_stress_test() {
    printf("\n--- 測試: 壓力測試 (多讀者多寫者) ---\n");
    
    printf("執行壓力測試：5個讀者，3個寫者，每個執行緒5次操作...\n");
    
    // 使用主程式的測試函數進行壓力測試
    run_reader_writer_test(5, 3, 5, 50, 100);
    
    TEST_ASSERT(1, "壓力測試完成"); // 如果程式沒有死鎖或崩潰，就算通過
}

/* ========== 效能測試 ========== */

void test_performance() {
    printf("\n--- 測試: 效能測試 ---\n");
    
    rw_lock_t lock;
    shared_database_t db;
    
    rw_lock_init(&lock);
    database_init(&db);
    
    clock_t start, end;
    const int iterations = 10000;
    
    // 測試讀者鎖效能
    start = clock();
    for (int i = 0; i < iterations; i++) {
        rw_lock_rdlock(&lock);
        rw_lock_rdunlock(&lock);
    }
    end = clock();
    
    double reader_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("讀者鎖操作 %d 次耗時: %.3f 秒\n", iterations, reader_time);
    
    // 測試寫者鎖效能
    start = clock();
    for (int i = 0; i < iterations; i++) {
        rw_lock_wrlock(&lock);
        rw_lock_wrunlock(&lock);
    }
    end = clock();
    
    double writer_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("寫者鎖操作 %d 次耗時: %.3f 秒\n", iterations, writer_time);
    
    TEST_ASSERT(reader_time < 5.0, "讀者鎖效能合理"); // 5秒內完成
    TEST_ASSERT(writer_time < 5.0, "寫者鎖效能合理"); // 5秒內完成
    
    rw_lock_destroy(&lock);
}

/* ========== 主測試函數 ========== */

int main(int argc, char *argv[]) {
    printf("=== 讀者-寫者問題測試套件 ===\n");
    printf("測試基於條件變數的讀者-寫者鎖實作\n");
    
    // 基本功能測試
    test_rw_lock_init_destroy();
    test_database_operations();
    test_basic_rw_lock_operations();
    
    // 同步測試
    test_multiple_readers();
    test_writer_exclusion();
    
    // 效能測試
    test_performance();
    
    // 壓力測試
    if (argc > 1 && strcmp(argv[1], "--stress") == 0) {
        test_stress_test();
    } else {
        printf("\n註: 執行 %s --stress 可進行壓力測試\n", argv[0]);
    }
    
    // 顯示測試結果
    print_test_results();
    
    return (test_failed > 0) ? 1 : 0;
} 