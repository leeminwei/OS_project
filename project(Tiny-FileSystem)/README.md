
# 🗂️ TinyFS — A User-Level Simulated File System
````
TinyFS 是一個模擬真實作業系統中文件儲存與管理機制的微型檔案系統，運行於使用者空間，並使用一個虛擬磁碟 (`mydisk.img`) 進行資料持久化。此專案為作業系統課程的實作項目，旨在學習檔案系統中的核心概念：如 inode、磁碟區塊分配、檔案指標、檔案操作 API 等。
````
## 📁 專案結構

```bash
.
├── TinyFS.h        # 檔案系統 API 介面定義
├── TinyFS.c        # TinyFS 檔案系統的核心實作
├── TinyFSApp.c     # 測試主程式，含多組自動測試案例
├── TinyDisk.h      # 虛擬磁碟的定義與常數
├── TinyDisk.c      # 虛擬磁碟 API 實作（不可修改）
├── Makefile        # 自動編譯腳本
└── README.md       # 專案說明文件（本檔）
````

---

## ⚙️ 系統架構圖

```
+-------------------------+
|       User App         |
|     (TinyFSApp.c)      |
+-----------+------------+
            |
            ↓
+-------------------------+
|       TinyFS API        |
|     (TinyFS.c)          |
| - File_Create / Write   |
| - File_Read / Delete    |
| - Inode / OFT 管理       |
+-----------+------------+
            |
            ↓
+-------------------------+
|     虛擬磁碟模組         |
|     (TinyDisk.c)        |
| - Disk_Read / Write     |
| - Disk_Save / Load      |
+-------------------------+
            ↓
      mydisk.img
 (虛擬磁碟實體映像檔)
```

---

## 📦 功能列表

### ✅ 核心功能

* [x] `FS_Boot()`：啟動檔案系統，載入或初始化磁碟
* [x] `File_Create()`：建立新檔案
* [x] `File_Open()`：開啟檔案並取得檔案描述符
* [x] `File_Write()`：寫入資料至檔案，支援跨多區塊
* [x] `File_Read()`：從檔案讀取資料，維持檔案指標
* [x] `File_Close()`：關閉檔案
* [x] `File_Delete()`：刪除檔案（需未開啟狀態）

### ⚠️ 限制與假設

* 每個檔案最多使用 5 個資料區塊（最大檔案大小為 5 × 512 bytes）
* 同時最多開啟 5 個檔案（OFT 限制）
* 最多可建立 100 個檔案（inode 限制）
* 不支援資料區塊回收（沒有 bitmap）

---

## 🚀 使用方式

### ✅ 編譯

進入專案目錄後執行：

```bash
make clean
make
```

### ✅ 執行測試

```bash
./demo mydisk.img
```

* 若 `mydisk.img` 不存在，將初始化一個新磁碟並寫入該檔案。
* 所有檔案的讀寫/刪除/錯誤測試將自動執行並顯示結果。

---

## 🧪 測試內容（由 `TinyFSApp.c` 自動執行）

* 檔案建立、重複建立偵測
* 開啟、關閉、重新開啟
* 寫入與讀取內容比對
* 部分讀寫與檔案指標變動
* 檔案大小上限測試
* 開啟檔案數上限（5 個）
* 刪除檔案與檢查是否在使用中
* 檢查錯誤情況（無效 FD、過長寫入等）

---

## 🔍 實作細節（摘要）

* `Inode` 結構包含 `valid`, `filename`, `size`, `data_region[5]`
* 每個 inode 對應最多 5 個資料區塊（直接指標）
* `Openfile` 結構追蹤目前開啟檔案與其 file pointer
* `FS_Boot()` 驗證 magic number 確認磁碟合法性

---

## 📥 產生的磁碟檔

執行完 `./demo mydisk.img` 後，會在資料夾中看到：

```bash
mydisk.img  ← 虛擬磁碟檔案
```

此檔案即為整個 TinyFS 的狀態，內含所有 inode、資料區塊、檔案資料。

---

## 🧠 延伸目標（Graduate only）

* `FS_Sync()`：將記憶體內容強制同步到磁碟
* `File_Seek(fd, offset)`：自訂檔案指標位置

---

## 🛠️ 開發與測試平台建議

* **系統**：Ubuntu 20.04 / WSL / 任一 UNIX-like 系統
* **編譯器**：`gcc` / `make`
* **注意**：請勿在 Windows 環境中編譯後執行於 WSL，可能會出現 `Exec format error`

---
