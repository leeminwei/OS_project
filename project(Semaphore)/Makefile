# 生產者消費者問題 - Makefile
# 編譯器設定
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
LDFLAGS = -pthread

# 目標檔案
TARGETS = producer_consumer_custom producer_consumer_system

# 物件檔案
CUSTOM_OBJS = producer_consumer_custom.o custom_semaphore.o utils.o
SYSTEM_OBJS = producer_consumer_system.o utils.o

# 預設目標
all: $(TARGETS)

# 編譯自製信號量版本
producer_consumer_custom: $(CUSTOM_OBJS)
	@echo "🔧 編譯自製信號量版本..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "✅ 自製信號量版本編譯完成"

# 編譯系統信號量版本
producer_consumer_system: $(SYSTEM_OBJS)
	@echo "🔧 編譯系統信號量版本..."
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "✅ 系統信號量版本編譯完成"

# 編譯物件檔案
%.o: %.c
	@echo "🔧 編譯 $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# 相依性規則
producer_consumer_custom.o: producer_consumer_custom.c common.h custom_semaphore.h
producer_consumer_system.o: producer_consumer_system.c common.h
custom_semaphore.o: custom_semaphore.c custom_semaphore.h
utils.o: utils.c common.h

# 執行測試
test: all
	@echo "🧪 執行測試..."
	@echo "📋 測試自製信號量版本："
	./producer_consumer_custom
	@echo ""
	@echo "📋 測試系統信號量版本："
	./producer_consumer_system

# 執行自製信號量版本
run-custom: producer_consumer_custom
	@echo "🚀 執行自製信號量版本..."
	./producer_consumer_custom

# 執行系統信號量版本
run-system: producer_consumer_system
	@echo "🚀 執行系統信號量版本..."
	./producer_consumer_system

# 清理
clean:
	@echo "🧹 清理編譯檔案..."
	rm -f $(TARGETS) *.o
	@echo "✅ 清理完成"

# 安裝（複製到系統目錄，可選）
install: all
	@echo "📦 安裝程式到 /usr/local/bin (需要管理員權限)..."
	sudo cp $(TARGETS) /usr/local/bin/
	@echo "✅ 安裝完成"

# 解除安裝
uninstall:
	@echo "🗑️ 從系統移除程式..."
	sudo rm -f /usr/local/bin/producer_consumer_custom
	sudo rm -f /usr/local/bin/producer_consumer_system
	@echo "✅ 解除安裝完成"

# 顯示幫助資訊
help:
	@echo "生產者消費者問題 - Makefile 幫助"
	@echo "=================================="
	@echo "可用目標："
	@echo "  all          - 編譯所有程式"
	@echo "  clean        - 清理編譯檔案"
	@echo "  test         - 執行兩個版本的測試"
	@echo "  run-custom   - 執行自製信號量版本"
	@echo "  run-system   - 執行系統信號量版本"
	@echo "  install      - 安裝到系統 (需要sudo)"
	@echo "  uninstall    - 從系統移除 (需要sudo)"
	@echo "  help         - 顯示此幫助資訊"
	@echo ""
	@echo "範例："
	@echo "  make all     # 編譯所有程式"
	@echo "  make test    # 測試兩個版本"
	@echo "  make clean   # 清理檔案"

# 重建（清理後重新編譯）
rebuild: clean all
	@echo "✅ 重建完成"

# 檢查程式碼風格（需要安裝 clang-format）
format:
	@echo "🎨 格式化程式碼..."
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i *.c *.h; \
		echo "✅ 程式碼格式化完成"; \
	else \
		echo "⚠️ clang-format 未安裝，跳過格式化"; \
	fi

# 檢查記憶體洩漏（需要安裝 valgrind）
valgrind-custom: producer_consumer_custom
	@echo "🔍 使用 Valgrind 檢查自製信號量版本..."
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --track-origins=yes ./producer_consumer_custom; \
	else \
		echo "⚠️ Valgrind 未安裝，跳過記憶體檢查"; \
	fi

valgrind-system: producer_consumer_system
	@echo "🔍 使用 Valgrind 檢查系統信號量版本..."
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --track-origins=yes ./producer_consumer_system; \
	else \
		echo "⚠️ Valgrind 未安裝，跳過記憶體檢查"; \
	fi

# 偽目標聲明
.PHONY: all clean test run-custom run-system install uninstall help rebuild format valgrind-custom valgrind-system 