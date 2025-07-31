# UNIX Shell – 作業系統課程專案

## 📚 專案簡介

本專案是一個簡易的 UNIX shell 實作，作為作業系統課程的練習。透過 `myshell` 指令列介面，使用者可以執行自訂的 UNIX 指令：`mycat`, `myhead`, `myuniq`。此 shell 支援基本指令解析、子行程執行、輸出重定向與錯誤處理。

---

## 🧾 功能說明

### 🔹 myshell

- 自訂 shell 介面，提示符號為：`myshell> `
- 解析使用者輸入並執行對應程式
- 支援：
  - fork 建立子行程
  - execvp 執行外部程式
  - waitpid 等待子行程結束
  - `exit` 內建指令結束 shell

### 🔹 mycat

- 顯示一或兩個檔案內容
- 用法：
```

./mycat file.txt
./mycat file1.txt file2.txt

```
- 若檔案不存在，顯示錯誤訊息

### 🔹 myhead

- 顯示檔案前 3 行內容
- 若檔案不足 3 行，顯示所有行
- 用法：
```

./myhead file.txt

```

### 🔹 myuniq

- 移除連續重複的行
- 支援 `-c` 選項，顯示每行出現次數
- 用法：
```

./myuniq file.txt
./myuniq -c file.txt

```

---

## 📤 輸出重定向（選擇性挑戰功能）

可使用 `-o` 將輸出導向至檔案，替代 `>` 功能。例如：

```

./mycat file.txt -o output.txt

````

此功能尚未完全實作於目前版本，可做為延伸挑戰。

---

## ⚠️ 錯誤處理

程式具備基本的防禦性設計，當輸入錯誤或檔案不存在時，會顯示提示而不導致程式崩潰：

- 檔案不存在 → 顯示 `"No such file or directory"`
- 參數錯誤 → 顯示 `"Too much file or directory"` 或 `"input error"`

---

## 🛠️ 編譯方式

請確保你已安裝 GCC 編譯器，然後使用以下指令進行編譯：

```bash
make
````

或手動編譯：

```bash
gcc -o myshell myshell.c
gcc -o mycat mycat.c
gcc -o myhead myhead.c
gcc -o myuniq myuniq.c
```

---

## ▶️ 執行範例

```bash
./myshell

myshell> ./mycat input.txt
myshell> ./myhead input.txt
myshell> ./myuniq input.txt
myshell> ./myuniq -c input.txt
myshell> exit
```

---

## 📁 專案結構

```
├── myshell.c      # 自訂 shell 主程式
├── mycat.c        # cat 指令模擬
├── myhead.c       # head 指令模擬
├── myuniq.c       # uniq 指令模擬
├── Makefile       # 編譯腳本
└── test.txt       # 測試用文字檔（請自行新增）
```

---

## 📌 學習重點

* 系統呼叫：`fork()`, `execvp()`, `waitpid()`
* 指令解析與參數處理
* 基本的 I/O 處理與錯誤處理
* 程式模組化與 CLI 互動

---

## 📝 License

本專案僅用於學術用途與作業展示，未授權作為商業使用。

