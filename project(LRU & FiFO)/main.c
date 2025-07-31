#include <stdio.h>
#include <stdlib.h>
#include "lru.h"
#include "fifo.h"
#include "utils.h"

typedef struct Node{
    int value;
    struct Node* next;
    struct Node* prev;
}Node;

typedef struct DLink{
    struct Node* head;
    struct Node* tail;
    int size;
    int capacity;
}DLink;

void print_array(int *arr, int size){
    for (int i=0 ; i<size ; i++) {
        printf("%d ", arr[i]);
    }
    return;
}

DLink* createDLink(int frame_count){
    DLink* LRU = (DLink*)malloc(sizeof(DLink));
    LRU->head = NULL;
    LRU->tail = NULL;
    LRU->size = 0;
    LRU->capacity = frame_count;
    return LRU;
}

void moveToHead(DLink* LRU, Node* node){
    //如果LRU為空或如果這個node是head
    if (LRU->head == NULL || node == LRU->head) {
        return;
    }
    //如果node是tail
    if (node == LRU->tail) {
        LRU->tail = LRU->tail->prev;
        LRU->tail->next = NULL;
        node->prev = NULL;
    }
    //如果node是中間節點
    if (node->next != NULL && node->prev != NULL) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next = NULL;
        node->prev = NULL;
    }
    node->next = LRU->head;
    LRU->head->prev = node;
    LRU->head = node;
    return;
}

void removeTail(DLink* LRU){
    if (LRU->head == NULL) {
        return;
    }
    Node* toRemove = LRU->tail;
    if (LRU->size == 1) {
        LRU->head = NULL;
        LRU->tail = NULL;
        LRU->size--;
        free(toRemove);
        return;
    }
    LRU->tail = LRU->tail->prev;
    LRU->tail->next = NULL;
    LRU->size--;
    free(toRemove);
    return;
}

void insertToHead(DLink* LRU, Node* node){
    if (LRU->head == NULL) {
        LRU->head = node;
        LRU->tail = node;
        LRU->size++;
        return;
    }
    node->next = LRU->head;
    LRU->head->prev = node;
    LRU->head = node;
    LRU->size++;
    return;
}

int is_page_in_frames(int page, int *frames, int frame_count){
    for (int i=0 ; i<frame_count ; i++) {
        if (page == frames[i]) {
            return 1;
        }
    }
    return 0;
}

int fifo_page_replacement(int *pages, int page_count, int frame_count) {
    DLink* fifo = createDLink(frame_count);
    int fault = page_count;
    int* frame = (int*)malloc(frame_count * sizeof(int));
    for (int i=0 ; i<page_count ; i++) {
        int status = 0;
        /*
        int check = is_page_in_frames(pages[i], frame, frame_count);
        //如果沒有重複元素
        if (check == 0) {

        }
        //如果有重複元素
        else{

        }
        */
        Node* cur = fifo->head;
        while (cur != NULL) {
            if (cur->value == pages[i]) {
                status = 1;
                fault--;
            }
            printf("%d ", cur->value);
            cur = cur->next;
        }
        printf("\n");
        //如果沒有重複項
        if (status == 0) {
            Node* newnode = (Node*)malloc(sizeof(Node));
            newnode->value = pages[i];
            newnode->next = NULL;
            newnode->prev = NULL;
            if (fifo->size == fifo->capacity) {
                removeTail(fifo);
            }
            insertToHead(fifo, newnode);
        }
    }
    return fault;
}

int lru_page_replacement(int *pages, int page_count, int frame_count) {
    DLink* LRU = createDLink(frame_count);
    int fault = page_count;
    for (int i=0 ; i<page_count ; i++) {
        Node* cur = LRU->head;
        int status = 0; //用來判斷是否有同樣的value在LRU
        while (cur != NULL) {
            if (cur->value == pages[i]) {
                status = 1;
                fault--;
                moveToHead(LRU, cur);
            }
            cur = cur->next;
        }
        //如果沒有重複元素
        if (status == 0) {
            //建立新的Node
            Node* newnode = (Node*)malloc(sizeof(Node));
            newnode->value = pages[i];
            newnode->next = NULL;
            newnode->prev = NULL;
            //如果LRU滿了
            if (LRU->size == LRU->capacity) {
                removeTail(LRU);
                insertToHead(LRU, newnode);
            }
            else{
                insertToHead(LRU, newnode);
            }
        }
    }
    return fault;
}

int main() {
    int k, i;
    int *pages;
    
    printf("=== 記憶體分頁演算法比較程式 ===\n");
    printf("比較LRU (最近最少使用) 與 FIFO (先進先出) 演算法\n\n");
    
    // 輸入可儲存片段數量
    printf("請輸入可儲存片段數量 k: ");
    scanf("%d", &k);
    
    if (k <= 0) {
        printf("錯誤：可儲存片段數量必須大於0\n");
        return 1;
    }
    
    // 輸入頁面請求數量
    printf("請輸入頁面請求數量 i: ");
    scanf("%d", &i);
    
    if (i <= 0) {
        printf("錯誤：頁面請求數量必須大於0\n");
        return 1;
    }
    
    // 分配記憶體存儲頁面請求序列
    pages = (int*)malloc(i * sizeof(int));
    if (pages == NULL) {
        printf("錯誤：記憶體分配失敗\n");
        return 1;
    }
    
    // 輸入頁面請求序列
    printf("請輸入 %d 個頁面請求序列: ", i);
    for (int j = 0; j < i; j++) {
        scanf("%d", &pages[j]);
    }
    
    printf("\n=== 輸入資料確認 ===\n");
    printf("可儲存片段數量: %d\n", k);
    printf("頁面請求數量: %d\n", i);
    printf("頁面請求序列: ");
    print_array(pages, i);
    printf("\n");
    
    // 執行FIFO演算法
    printf("\n=== FIFO (先進先出) 演算法結果 ===\n");
    int fifo_faults = fifo_page_replacement(pages, i, k);
    
    // 執行LRU演算法
    printf("\n=== LRU (最近最少使用) 演算法結果 ===\n");
    int lru_faults = lru_page_replacement(pages, i, k);
    
    
    // 比較結果
    printf("\n=== 演算法比較結果 ===\n");
    printf("FIFO 頁面錯誤次數: %d\n", fifo_faults);
    printf("LRU 頁面錯誤次數:  %d\n", lru_faults);
    
    if (fifo_faults < lru_faults) {
        printf("結論: FIFO 演算法表現較佳 (頁面錯誤次數較少)\n");
    } else if (lru_faults < fifo_faults) {
        printf("結論: LRU 演算法表現較佳 (頁面錯誤次數較少)\n");
    } else {
        printf("結論: 兩種演算法表現相同\n");
    }
    
    printf("頁面錯誤率 - FIFO: %.2f%%, LRU: %.2f%%\n", 
           (double)fifo_faults / i * 100, 
           (double)lru_faults / i * 100);
    
    // 釋放記憶體
    free(pages);
    
    return 0;
} 