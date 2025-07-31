# 🧵 生產者－消費者問題（C語言實作）

本專案展示了兩種版本的經典\*\*生產者－消費者問題（Producer-Consumer Problem）\*\*解法：

1. ✅ 使用 **系統提供的 POSIX 信號量與互斥鎖**
2. 🧪 使用 **自製的信號量（以 `pthread_cond` + `pthread_mutex` 模擬）**

透過\*\*有界緩衝區（bounded buffer）\*\*模擬資料交換，實作多執行緒間同步與互斥控制，避免競爭條件與資源衝突。

---

## 📂 專案結構

```
.
├── Assignment.md                   # 作業需求說明文件
├── common.h                        # 共用常數與結構定義
├── custom_semaphore.c             # 自製信號量實作
├── producer_consumer_custom.c     # 使用自製信號量的主程式
├── producer_consumer_system.c     # 使用 POSIX 系統信號量的主程式
├── utils.c                         # 公用函式（時間戳、印出狀態等）
├── Makefile                        # 編譯腳本
```

---

## ⚙️ 編譯方式

可使用 Makefile 一鍵編譯：

```bash
make            # 同時編譯 system/custom 版本
make clean      # 清除所有可執行檔與中間檔
```

也可手動編譯：

```bash
gcc -o system_version producer_consumer_system.c utils.c -lpthread
gcc -o custom_version producer_consumer_custom.c custom_semaphore.c utils.c -lpthread
```

---

## 🚀 執行方式

### 1. 系統信號量版本

```bash
./system_version
```

### 2. 自製信號量版本

```bash
./custom_version
```

程式將模擬生產與消費行為，並在終端輸出緩衝區狀態與操作流程。

---

## 🔒 同步機制說明

| 機制              | 說明                      |
| --------------- | ----------------------- |
| `empty_slots`   | 表示剩餘可放入的空格數             |
| `full_slots`    | 表示已放入的項目數，可供消費者取用       |
| `pthread_mutex` | 用來保護共享資源（buffer）區塊的互斥存取 |

### 自製信號量邏輯：

使用 `value` 代表可用資源數，透過 `pthread_cond_wait()` 等待資源、`pthread_cond_signal()` 喚醒等待者，模擬 `sem_wait()` / `sem_post()` 行為。

---

## 🧪 範例輸出（部分）

```
[10:15:00.123] 生產者放入：1 | 緩衝區：[1, -, -, -, -, -, -, -]
[10:15:00.234] 消費者取出：1 | 緩衝區：[-, -, -, -, -, -, -, -]
```

---

## 📘 學習重點

* 理解條件變數與互斥鎖的使用時機
* 自製同步原語（模擬 semaphore）
* 多執行緒程式中 race condition 的處理
* POSIX 執行緒與同步工具的實務操作

---

## 🧠 延伸思考

* 如何支援多個生產者與多個消費者？
* 如何加入優先級或公平策略避免飢餓？
* 若不使用 busy-wait，有哪些同步機制可用？

---

## 📜 版權與用途

本專案僅供學術用途與課程實作練習使用，如需延伸應用請註明出處。

---
