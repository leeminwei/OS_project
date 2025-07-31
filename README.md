
---

# 🧠 作業系統專案總覽

本倉庫包含 7 個使用 C 語言撰寫的系統層級專案，涵蓋了多執行緒同步（Mutex、Semaphore、Condition Variable）、LRU\&FIFO 快取策略模擬、UNIX Shell、Tiny FileSystem、Tiny Scheduler 等核心作業系統主題。每個專案皆可獨立編譯執行，具備完整測試範例與 Makefile 腳本。

---

## 📁 專案總覽

| 專案名稱                 | 主題                 | 說明                                                 |
| -------------------- | ------------------ | -------------------------------------------------- |
| `Condition_Variable` | 多執行緒同步             | 實作 Readers-Writers 問題，使用 `pthread_cond_t` 管理讀寫互斥邏輯 |
| `Mutex`              | Mutex 操作           | 使用 pthread mutex 實作計數器保護                           |
| `Semaphore`          | 自製與系統 Semaphore 比較 | 各自使用 POSIX semaphore 與自製 semaphore 同步生產者與消費者       |
| `LRU_FIFO`           | Cache 排程策略模擬       | 模擬 LRU 及 FIFO 資料快取機制                               |
| `UNIX_shell`         | 簡易 UNIX Shell      | 支援指令解析、fork/exec 執行、內建指令處理                         |
| `Tiny_filesystem`    | TinyFS 小型檔案系統      | 建立模擬磁碟映像檔，支援檔案建立、刪除、存取                             |
| `Tiny_Scheduler`     | CPU 排程模擬           | 模擬 FCFS、RR 等排程演算法並輸出狀態紀錄                           |

---

## 🔧 編譯方式

每個子目錄下皆附有 `Makefile`，可直接透過以下指令完成編譯：

```bash
cd 子專案資料夾名稱
make
```

編譯成功後會在當前目錄產生執行檔，例如：

```bash
./main
```

若需清除舊檔案，可使用：

```bash
make clean
```

---

## 🧪 測試方式

部分專案附有測資檔（如 `testcase.txt`、`tests.txt`、`interactive_test.txt`），可配合標準輸入或重導向執行測試：

```bash
./main < testcase.txt
```

UNIX shell 專案亦支援互動式測試，Tiny Scheduler 則會輸出各時間單位內的狀態更新。

---

## 📌 子專案重點摘要

### 🔹 `Condition_Variable/`

* 主題：Readers-Writers 同步問題
* 技術：pthread\_cond\_t、互斥鎖、條件變數
* 特點：支援多位讀者/寫者、讀寫公平性控制

### 🔹 `Mutex/`

* 主題：共享資源保護
* 實作一個使用 `pthread_mutex_t` 的安全遞增計數器
* 多執行緒下保持正確性

### 🔹 `Semaphore/`

* 比較兩種 Semaphore 實作：

  * 使用 `<semaphore.h>` 系統信號量（`sem_t`）
  * 自製 Semaphore：使用 pthread + value + cond
* 模擬生產者與消費者問題，觀察同步效果

### 🔹 `LRU_FIFO/`

* 主題：快取模擬
* 支援：

  * FIFO 策略（先進先出）
  * LRU 策略（最近最少使用）
* 輸入：記憶體參照序列
* 輸出：命中率、缺頁率、交換紀錄

### 🔹 `UNIX_shell/`

* 模擬 shell 行為
* 功能包含：

  * 內建命令（如 `exit`）
  * 外部命令解析與執行（`execvp`）
  * 支援管線 (`|`)、I/O 重導向（選配）

### 🔹 `Tiny_filesystem/`

* 建立在記憶體上的模擬檔案系統
* 支援檔案：

  * 建立 / 刪除 / 寫入 / 讀取
* 類似 Linux VFS 概念，具 `disk image` 操作邏輯

### 🔹 `Tiny_Scheduler/`

* 模擬：

  * FCFS（先來先服務）
  * RR（Round Robin）
* 支援 process 到達、執行、離開的完整模擬流程
* 可輸出每個時間點的 CPU 狀態與 ready queue

---

## 📦 套件需求

本專案均使用 C 語言與 pthread library 實作：

* GCC 或 Clang 編譯器
* POSIX Threads (`pthread`)
* Make 工具

---

## 🚀 執行範例

```bash
cd Condition_Variable/
make
./main 3 2 5 100 150  # 執行 3 個讀者、2 個寫者，每人操作 5 次
```

```bash
cd LRU_FIFO/
make
./main < tests.txt
```

---

