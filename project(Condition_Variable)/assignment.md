# 讀者-寫者問題實習作業詳細要求

## 作業概述

本作業要求實作經典的**讀者-寫者同步問題**，使用條件變數（Condition Variables）和互斥鎖（Mutex）來確保正確的並行存取控制。

## 學習目標

完成此作業後，應能夠：

1. **理解條件變數的概念和使用方法**
2. **掌握讀者-寫者問題的同步需求**
3. **實作無死鎖和無饑餓的同步解決方案**
4. **應用多執行緒程式設計的最佳實踐**
5. **進行同步程式的測試和除錯**

## 問題規格

### 同步規則

1. **讀者同時存取**: 多個讀者可以同時讀取共享資料
2. **寫者獨占存取**: 寫者必須獨占存取共享資料
3. **讀寫互斥**: 讀者和寫者不能同時存取共享資料
4. **避免饑餓**: 確保讀者和寫者都能最終獲得存取權

### 優先權策略

本作業採用**寫者優先策略**：
- 當有寫者等待時，新的讀者必須等待
- 這樣可以避免寫者被連續的讀者請求餓死

## 實作要求

### 必須實作的功能

#### 1. 讀者-寫者鎖 (rw_lock_t)

```c
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
```

#### 2. 核心函數

- `rw_lock_init()`: 初始化讀者-寫者鎖
- `rw_lock_destroy()`: 銷毀讀者-寫者鎖
- `rw_lock_rdlock()`: 獲取讀者鎖
- `rw_lock_rdunlock()`: 釋放讀者鎖
- `rw_lock_wrlock()`: 獲取寫者鎖
- `rw_lock_wrunlock()`: 釋放寫者鎖

#### 3. 執行緒函數

- `reader_thread()`: 讀者執行緒的主要邏輯
- `writer_thread()`: 寫者執行緒的主要邏輯

#### 4. 共享資源模擬

實作一個模擬的共享資料庫，包含：
- 資料內容存儲
- 版本號追蹤
- 時間戳記錄

### 實作細節要求

#### 1. 條件變數使用

**必須正確使用while迴圈**：
```c
// ✅ 正確做法
while (condition_not_met) {
    pthread_cond_wait(&cond, &mutex);
}

// ❌ 錯誤做法  
if (condition_not_met) {
    pthread_cond_wait(&cond, &mutex);
}
```

**原因**: 避免虛假喚醒（spurious wakeup）問題

#### 2. 鎖的獲取順序

**讀者鎖獲取邏輯**：
```c
pthread_mutex_lock(&rw_lock->mutex);
while (rw_lock->writers_count > 0 || rw_lock->waiting_writers > 0) {
    pthread_cond_wait(&rw_lock->read_cond, &rw_lock->mutex);
}
rw_lock->readers_count++;
pthread_mutex_unlock(&rw_lock->mutex);
```

**寫者鎖獲取邏輯**：
```c
pthread_mutex_lock(&rw_lock->mutex);
rw_lock->waiting_writers++;
while (rw_lock->readers_count > 0 || rw_lock->writers_count > 0) {
    pthread_cond_wait(&rw_lock->write_cond, &rw_lock->mutex);
}
rw_lock->waiting_writers--;
rw_lock->writers_count++;
pthread_mutex_unlock(&rw_lock->mutex);
```

#### 3. 條件變數通知策略

**讀者釋放時**：
```c
if (rw_lock->readers_count == 0) {
    pthread_cond_signal(&rw_lock->write_cond);  // 通知寫者
}
```

**寫者釋放時**：
```c
if (rw_lock->waiting_writers > 0) {
    pthread_cond_signal(&rw_lock->write_cond);  // 優先通知寫者
} else {
    pthread_cond_broadcast(&rw_lock->read_cond); // 通知所有讀者
}
```

### 錯誤處理要求

1. **NULL指標檢查**: 所有函數都必須檢查NULL指標
2. **系統呼叫錯誤**: 適當處理pthread函數的回傳值
3. **記憶體管理**: 確保沒有記憶體洩漏

### 程式碼品質要求

1. **註解說明**: 重要邏輯都要有清楚的註解
2. **變數命名**: 使用有意義的變數名稱
3. **程式碼格式**: 遵循一致的縮排和格式
4. **模組化設計**: 功能應適當分離

## 測試要求

### 必須通過的測試

1. **基本功能測試**
   - 初始化和銷毀測試
   - 單一讀者/寫者測試
   - 錯誤處理測試

2. **同步正確性測試**
   - 多讀者同時存取測試
   - 寫者獨占存取測試
   - 讀寫互斥測試

3. **效能測試**
   - 大量操作的效能測試
   - 記憶體使用檢查

4. **壓力測試**
   - 多讀者多寫者並行測試
   - 長時間運行穩定性測試

### 測試場景範例

```bash
# 基本測試
make test

# 壓力測試
make stress-test

# 記憶體檢查
make memcheck

# 自訂參數測試
./reader_writer 5 3 10 100 200
# 5個讀者, 3個寫者, 每個執行緒10次操作, 讀者延遲100ms, 寫者延遲200ms
```


## 提交要求

### 必須提交的檔案

```
CV/
├── README.md              # 作業說明
├── assignment.md          # 本檔案
├── reader_writer.h        # 標頭檔案
├── reader_writer.c        # 主程式實作
├── test_reader_writer.c   # 測試程式
├── Makefile              # 編譯腳本
```



## 常見陷阱和注意事項

### 1. 虛假喚醒問題
```c
// 錯誤：使用if
if (condition) {
    pthread_cond_wait(&cond, &mutex);
}

// 正確：使用while
while (condition) {
    pthread_cond_wait(&cond, &mutex);
}
```

### 2. 死鎖避免
- 始終以相同順序獲取多個鎖
- 避免在持有鎖時呼叫可能阻塞的函數
- 確保每個pthread_mutex_lock都有對應的unlock

### 3. 饑餓問題
- 實作公平的調度策略
- 避免讀者連續請求導致寫者饑餓
- 考慮使用優先權機制

### 4. 記憶體管理
- 確保pthread_create的每個執行緒都有pthread_join
- 正確初始化和銷毀同步原語
- 檢查記憶體分配的成功性
