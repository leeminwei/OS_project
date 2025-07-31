#ifndef FIFO_H
#define FIFO_H

// FIFO 頁面替換演算法
// 參數: pages - 頁面請求序列, page_count - 頁面請求數量, frame_count - 記憶體框架數量
// 返回: 頁面錯誤次數
int fifo_page_replacement(int *pages, int page_count, int frame_count);

#endif 