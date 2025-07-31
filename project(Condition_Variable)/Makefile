# 讀者-寫者問題實習作業 Makefile
# 基於條件變數的同步機制實作

# 編譯器設定
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread -g -O0
LDFLAGS = -pthread

# 檔案定義
HEADER = reader_writer.h
MAIN_SRC = reader_writer.c
TEST_SRC = test_reader_writer.c
MAIN_OBJ = reader_writer.o
TEST_OBJ = test_reader_writer.o

# 目標檔案
MAIN_TARGET = reader_writer
TEST_TARGET = test_reader_writer

# 預設目標
.PHONY: all clean test help debug release

all: $(MAIN_TARGET) $(TEST_TARGET)

# 主程式編譯
$(MAIN_TARGET): $(MAIN_OBJ)
	@echo "🔨 連結主程式..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "✅ 主程式編譯完成: $(MAIN_TARGET)"

$(MAIN_OBJ): $(MAIN_SRC) $(HEADER)
	@echo "🔨 編譯主程式..."
	$(CC) $(CFLAGS) -c $< -o $@

# 測試程式編譯  
$(TEST_TARGET): $(TEST_OBJ) reader_writer_lib.o
	@echo "🔨 連結測試程式..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "✅ 測試程式編譯完成: $(TEST_TARGET)"

$(TEST_OBJ): $(TEST_SRC) $(HEADER)
	@echo "🔨 編譯測試程式..."
	$(CC) $(CFLAGS) -c $< -o $@

# 建立函式庫版本（不包含main函數）
reader_writer_lib.o: $(MAIN_SRC) $(HEADER)
	@echo "🔨 編譯函式庫..."
	$(CC) $(CFLAGS) -DLIB_BUILD -c $< -o $@

# 執行測試
test: $(TEST_TARGET)
	@echo "🧪 執行基本測試..."
	./$(TEST_TARGET)
	@echo ""

# 執行壓力測試
stress-test: $(TEST_TARGET)
	@echo "🧪 執行壓力測試..."
	./$(TEST_TARGET) --stress

# 執行主程式（預設參數）
run: $(MAIN_TARGET)
	@echo "🚀 執行主程式（預設參數）..."
	./$(MAIN_TARGET)

# 執行主程式（自訂參數）
run-custom: $(MAIN_TARGET)
	@echo "🚀 執行主程式（自訂參數）..."
	@echo "格式: ./$(MAIN_TARGET) [讀者數] [寫者數] [操作次數] [讀者延遲ms] [寫者延遲ms]"
	@read -p "讀者數量 (預設3): " readers && \
	 read -p "寫者數量 (預設2): " writers && \
	 read -p "操作次數 (預設3): " ops && \
	 read -p "讀者延遲ms (預設100): " r_delay && \
	 read -p "寫者延遲ms (預設200): " w_delay && \
	 ./$(MAIN_TARGET) $${readers:-3} $${writers:-2} $${ops:-3} $${r_delay:-100} $${w_delay:-200}

# 除錯版本
debug: CFLAGS += -DDEBUG -g3 -O0
debug: clean $(MAIN_TARGET) $(TEST_TARGET)
	@echo "🐛 除錯版本編譯完成"

# 發布版本
release: CFLAGS += -O2 -DNDEBUG
release: clean $(MAIN_TARGET) $(TEST_TARGET)
	@echo "🚀 發布版本編譯完成"

# 使用valgrind檢查記憶體洩漏
memcheck: $(MAIN_TARGET) $(TEST_TARGET)
	@echo "🔍 使用valgrind檢查記憶體..."
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "檢查主程式:"; \
		valgrind --leak-check=full --show-leak-kinds=all \
			--track-origins=yes ./$(MAIN_TARGET) 2 2 2 50 50; \
		echo "檢查測試程式:"; \
		valgrind --leak-check=full --show-leak-kinds=all \
			--track-origins=yes ./$(TEST_TARGET); \
	else \
		echo "⚠️  valgrind 未安裝，跳過記憶體檢查"; \
	fi

# 使用gdb除錯
gdb: $(MAIN_TARGET)
	@echo "🐛 啟動gdb除錯器..."
	gdb ./$(MAIN_TARGET)

# 編譯資訊
info:
	@echo "📋 編譯資訊:"
	@echo "編譯器: $(CC)"
	@echo "編譯選項: $(CFLAGS)"
	@echo "連結選項: $(LDFLAGS)"
	@echo "原始檔: $(MAIN_SRC) $(TEST_SRC)"
	@echo "標頭檔: $(HEADER)"
	@echo "目標檔: $(MAIN_TARGET) $(TEST_TARGET)"

# 程式碼統計
stats:
	@echo "📊 程式碼統計:"
	@echo "標頭檔案:"
	@wc -l $(HEADER)
	@echo "主程式檔案:"
	@wc -l $(MAIN_SRC)
	@echo "測試檔案:"
	@wc -l $(TEST_SRC)
	@echo "總計:"
	@wc -l $(HEADER) $(MAIN_SRC) $(TEST_SRC) | tail -1

# 檢查程式碼風格
style-check:
	@echo "🎨 檢查程式碼風格..."
	@if command -v clang-format >/dev/null 2>&1; then \
		echo "使用clang-format檢查風格:"; \
		clang-format --dry-run --Werror $(MAIN_SRC) $(TEST_SRC) $(HEADER) 2>&1 || \
		echo "⚠️  程式碼風格需要調整"; \
	else \
		echo "⚠️  clang-format 未安裝，跳過風格檢查"; \
	fi

# 自動格式化程式碼
format:
	@echo "🎨 格式化程式碼..."
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i $(MAIN_SRC) $(TEST_SRC) $(HEADER); \
		echo "✅ 程式碼格式化完成"; \
	else \
		echo "⚠️  clang-format 未安裝，無法格式化"; \
	fi

# 完整測試流程
full-test: clean all test stress-test
	@echo "🎉 完整測試流程完成!"

# 清理
clean:
	@echo "🧹 清理編譯檔案..."
	rm -f $(MAIN_OBJ) $(TEST_OBJ) reader_writer_lib.o
	rm -f $(MAIN_TARGET) $(TEST_TARGET)
	rm -f core vgcore.*
	@echo "✅ 清理完成"

# 說明
help:
	@echo "📖 可用的Makefile目標:"
	@echo ""
	@echo "🔨 編譯相關:"
	@echo "  all          - 編譯所有程式（預設）"
	@echo "  debug        - 編譯除錯版本"
	@echo "  release      - 編譯發布版本"
	@echo "  clean        - 清理編譯檔案"
	@echo ""
	@echo "🧪 測試相關:"
	@echo "  test         - 執行基本測試"
	@echo "  stress-test  - 執行壓力測試"
	@echo "  full-test    - 執行完整測試流程"
	@echo "  memcheck     - 記憶體洩漏檢查（需要valgrind）"
	@echo ""
	@echo "🚀 執行相關:"
	@echo "  run          - 執行主程式（預設參數）"
	@echo "  run-custom   - 執行主程式（自訂參數）"
	@echo "  gdb          - 使用gdb除錯"
	@echo ""
	@echo "📊 資訊相關:"
	@echo "  info         - 顯示編譯資訊"
	@echo "  stats        - 顯示程式碼統計"
	@echo "  style-check  - 檢查程式碼風格"
	@echo "  format       - 自動格式化程式碼"
	@echo "  help         - 顯示此說明"
	@echo ""
	@echo "💡 範例用法:"
	@echo "  make all && make test       # 編譯並測試"
	@echo "  make run                    # 執行預設程式"
	@echo "  make clean && make debug    # 清理並編譯除錯版本"

# 防止make把這些當作檔案名稱
.PHONY: all clean test stress-test run run-custom debug release memcheck gdb info stats style-check format full-test help 