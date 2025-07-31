#ifndef UTILS_H
#define UTILS_H

// 列印陣列函數
void print_array(int *arr, int size);

// 列印記憶體框架狀態
void print_frames(int *frames, int frame_count);

// 檢查頁面是否在記憶體框架中
int is_page_in_frames(int page, int *frames, int frame_count);

// 尋找頁面在框架中的位置
int find_page_index(int page, int *frames, int frame_count);

// 初始化框架 (設為-1表示空)
void initialize_frames(int *frames, int frame_count);

#endif 