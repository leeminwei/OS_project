# Makefile for 記憶體分頁演算法比較程式

# 編譯器設定
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# 目標執行檔案名稱
TARGET = page_replacement

# 原始檔案
SOURCES = main.c utils.c fifo.c lru.c

# 物件檔案
OBJECTS = $(SOURCES:.c=.o)

# 頭檔案
HEADERS = utils.h fifo.h lru.h

# 預設目標
all: $(TARGET)

# 連結目標
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "編譯完成！執行檔案: $(TARGET)"

# 編譯物件檔案
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理編譯產生的檔案
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "清理完成！"

# 重新編譯
rebuild: clean all

# 運行程式
run: $(TARGET)
	./$(TARGET)

# 顯示幫助訊息
help:
	@echo "可用的目標:"
	@echo "  all     - 編譯程式 (預設)"
	@echo "  clean   - 清理編譯檔案"
	@echo "  rebuild - 重新編譯"
	@echo "  run     - 編譯並執行程式"
	@echo "  help    - 顯示此幫助訊息"

.PHONY: all clean rebuild run help 