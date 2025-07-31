# 🧵 執行緒安全計數器 (`counter_t`) 專案

本專案示範如何使用 **pthread mutex (互斥鎖)** 實作一個支援多執行緒安全操作的整數計數器。該計數器可在高併發環境中防止資料競爭與不一致問題，適合作為學習 pthread 多執行緒同步技術的基礎範例。

---

## 📌 專案目標

* 實作一個 `counter_t` 結構，支援：

  * 初始化與銷毀
  * 遞增指定數值
  * 安全取得目前值
* 練習 pthread 中 `mutex` 的基本使用
* 強化對 Race Condition 的理解與處理方式

---

## 📁 專案結構

```
.
├── counter.h              # 計數器結構與函式原型宣告
├── counter.c              # 計數器實作檔（加入 mutex 操作）
├── tests/
│   └── test_counter.c     # 多執行緒測試程式
├── Makefile               # 編譯與測試腳本
└── README.md              # 專案說明文件（本檔）
```

---

## 🔧 編譯與執行

### 🔨 編譯

```bash
make
```

### ▶️ 執行測試

```bash
make test
```

執行後若成功，會看到：

```
Expected 4000000, got 4000000
Test passed!
```

---

## 📘 使用說明與邏輯設計

### 1. counter\_t 結構包含：

```c
typedef struct {
    int value;                 // 計數器值
    pthread_mutex_t lock;      // 互斥鎖
} counter_t;
```

### 2. 功能實作重點

* `counter_init()`：

  * 設定初始值
  * 初始化 mutex

* `counter_increment()`：

  * 使用 `pthread_mutex_lock()` / `unlock()` 包住加總操作

* `counter_get()`：

  * 若想確保資料一致性，建議也包住 mutex（視需求而定）

* `counter_destroy()`：

  * 清除 mutex 資源

---

## 🔬 測試說明

* 測試程式中會產生大量執行緒（例如 4 個執行緒，每個迴圈 100 萬次）
* 若無 mutex 保護，會出現 **race condition** 導致結果錯誤
* 正確加上 mutex 後，最終加總應完全正確

---

## 📝 注意事項

* 請勿使用 `pthread_cond_*` 或 semaphore
* 請確保所有共享資源操作皆在 mutex 保護下進行
* 程式中請妥善釋放所有資源（mutex）

---
