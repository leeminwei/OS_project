# 作業：Mutex 與執行緒安全計數器

## 目標

實作一個「執行緒安全計數器」(`counter_t`) ，利用 *互斥鎖 (mutex)* 來確保多執行緒同時操作時資料的一致性。本作業 **禁止** 使用條件變數 (`pthread_cond_*`) 與號誌 (semaphore)，因為相關內容尚未教學。

## 檔案說明

- `counter.h`：計數器資料結構及 API 介面。
- `counter.c`：計數器函式庫的實作，預留 `TODO` 供同學撰寫。
- `tests/test_counter.c`：基本測試，將產生多執行緒同時遞增計數器並驗證最終結果。
- `Makefile`：提供 `make`、`make test`、`make clean` 等目標以編譯並執行測試。

## 任務

1. 完成 `counter.c` 中所有 `TODO`。
   * 應在 `counter_init()` 中初始化 mutex 與初始值。
   * 在 `counter_increment()` 與 `counter_get()` 中加入適當的 `pthread_mutex_lock()` / `pthread_mutex_unlock()`。
   * 在 `counter_destroy()` 中銷毀 mutex。
2. 執行 `make test` 後，程式應印出類似結果並通過 `assert`：
   ```
   Expected 4000000, got 4000000
   Test passed!
   ```
   (數字會依常數而異，但應相符)
3. 若想進行壓力測試，可自行調整 `THREADS` 與 `LOOPS` 常數。

## 提交

提交下列檔案：

- `counter.h`
- `counter.c`

若你有修改測試或 Makefile 亦請一併提交。 