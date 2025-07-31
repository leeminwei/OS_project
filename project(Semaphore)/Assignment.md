# 鎖與條件變數作業 - 生產者消費者問題

## 專案簡介
本專案實作經典的生產者消費者問題，使用兩種不同的信號量方式：
1. **自定義信號量** (`custom_semaphore.c`) - 使用互斥鎖和條件變數實現
2. **系統信號量** (`producer_consumer_system.c`) - 使用POSIX信號量

## 任務清單

### 📋 必須完成的函數實現

#### 1. 自定義信號量實現 (`custom_semaphore.c`)
- `custom_sem_init()` - 初始化信號量
- `custom_sem_destroy()` - 銷毀信號量  
- `custom_sem_wait()` - P操作（等待操作）
- `custom_sem_post()` - V操作（信號操作）
- `custom_sem_getvalue()` - 獲取信號量值

#### 2. 工具函數實現 (`utils.c`)
- `print_timestamp()` - 列印時間戳
- `get_timestamp_ms()` - 獲取毫秒時間戳
- `print_buffer_status()` - 列印緩衝區狀態

#### 3. 緩衝區操作函數
在 `producer_consumer_custom.c` 和 `producer_consumer_system.c` 中：
- `init_buffer()` - 初始化環形緩衝區
- `buffer_put()` - 向緩衝區添加項目
- `buffer_get()` - 從緩衝區取出項目

## 📚 重要概念

### 信號量操作
- **P操作 (wait)**：如果信號量值 > 0，則減1；否則阻塞等待
- **V操作 (post)**：信號量值加1，並喚醒等待的執行緒

### 環形緩衝區
- 使用 `in` 和 `out` 指標管理讀寫位置
- 使用模運算 `% BUFFER_SIZE` 實現環形結構
- 使用 `count` 記錄當前項目數量

### 同步機制
- **empty_slots** 信號量：記錄可用的空閒槽位
- **full_slots** 信號量：記錄已有數據的槽位  
- **buffer_mutex** 互斥鎖：保護緩衝區的臨界區

## 🔧 編譯和執行

```bash
# 編譯所有程式
make all

# 執行自定義信號量版本
./producer_consumer_custom

# 執行系統信號量版本  
./producer_consumer_system

# 清理編譯文件
make clean
```

## ✅ 完成標準

1. 所有TODO項目均已實現
2. 程式能正確編譯並執行
3. 生產者和消費者能正確同步
4. 沒有競態條件或死鎖現象
5. 緩衝區狀態顯示正確

## 💡 實現提示

### 條件變數使用
```c
// 等待條件
while (condition_not_met) {
    pthread_cond_wait(&condition, &mutex);
}

// 喚醒等待者
pthread_cond_signal(&condition);
```

### 環形緩衝區索引計算
```c
next_index = (current_index + 1) % BUFFER_SIZE;
```

### 時間處理
```c
struct timeval tv;
gettimeofday(&tv, NULL);
// tv.tv_sec: 秒
// tv.tv_usec: 微秒
```

## 🎯 學習目標

完成本作業後，您將掌握：
- 互斥鎖和條件變數的正確使用
- 信號量的實現原理和應用
- 生產者消費者問題的解決方案
- 執行緒同步和通信機制
- 環形緩衝區的設計和實現 