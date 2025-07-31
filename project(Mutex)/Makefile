# 用於組建作業的 Makefile
CC = gcc
CFLAGS = -Wall -Wextra -pthread -g

SRC = counter.c
HDR = counter.h
TEST = tests/test_counter.c
OBJ = $(SRC:.c=.o)

TARGET = test_counter

all: $(TARGET)

$(TARGET): $(OBJ) $(TEST)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(TEST)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean test 