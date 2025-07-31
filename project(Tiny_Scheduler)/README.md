

---

## 📄 Tiny Scheduler：CPU 排程模擬器（支援 FCFS 與 RR）

這是一個用 C 語言實作的簡易 CPU 排程模擬器，支援以下兩種經典排程演算法：

* **先到先服務（FCFS）** — 非搶佔式
* **時間片輪轉（RR）** — 搶佔式，可自訂時間片大小

模擬器從指定的輸入文字檔讀取行程（Process）資料，從模擬時間 0 開始逐步排程，並輸出每個時間區間中執行中的行程與就緒佇列內容。

---

### 📁 專案檔案結構

```
.
├── tsim.c           // 主程式，實作 FCFS 與 RR 演算法
├── utils.h         // 結構定義（如 Process 結構）
├── input1.txt      // 測試用輸入檔案
├── input2.txt
├── input3.txt
├── Makefile        // 已提供，請勿修改
├── tester.c        // 已提供，用來處理參數解析與測試
```

---

### ⚙️ 編譯方式

使用提供的 `Makefile` 進行編譯：

```bash
make
```

會產生一個可執行檔 `tsim`。

---

### ▶️ 執行方式

```bash
./tsim fcfs input_filename
./tsim rr input_filename time_slice
```

* `fcfs`：使用先到先服務排程
* `rr`：使用時間片輪轉排程，需額外提供 `time_slice`（整數）
* `input_filename`：包含行程清單的輸入檔

---

### 📄 輸入檔格式（每行一筆行程）

```
PID Arrival_Time Run_Time
```

範例：

```
P1 0 5
P2 2 3
P3 4 2
```

* `PID`：行程代號（字串）
* `Arrival_Time`：到達時間（毫秒）
* `Run_Time`：執行時間（毫秒）

---

### 🖨️ 範例輸出格式

```
Running FCFS
Time | Running | Ready Queue
-----------------------------
 0 - 1 | P1     | -
 1 - 2 | P1     | P2
 2 - 3 | P1     | P2 P3
...
FCFS Execution Completed.
Average Response Time: 4.60 ms
Average Turnaround Time: 8.60 ms
Total Context Switches: 4
```

---

### ⚠️ 錯誤處理說明

* 若未提供足夠參數、未指定排程策略，會顯示用法說明。
* 若開啟檔案失敗或格式錯誤，會顯示對應的錯誤訊息。
* 本模擬器支援的行程數量為 2\~5 筆。

---

### ✅ 支援功能總覽

| 功能                          | 支援 |
| --------------------------- | -- |
| FCFS 非搶佔式排程                 | ✅  |
| RR 時間片輪轉排程                  | ✅  |
| Ready Queue 視覺化輸出           | ✅  |
| IDLE 狀態顯示                   | ✅  |
| 上下文切換統計                     | ✅  |
| 平均 Response / Turnaround 計算 | ✅  |

