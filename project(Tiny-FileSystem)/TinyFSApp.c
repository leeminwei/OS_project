#include "TinyFS.h"
#include "TinyDisk.h"
#include <string.h>
#include <stdio.h>

void usage(char *prog){
    fprintf(stderr, "usage: %s <disk image file>\n", prog);
    exit(1);
}

// 輸出測試結果
void test_result(char* test_name, int result, int expected) {
    if(result == expected) {
        printf("✓ %s 通過\n", test_name);
    } else {
        printf("✗ %s 失敗 (結果: %d, 預期: %d)\n", test_name, result, expected);
    }
}

// 測試基本檔案操作
void test_basic_operations() {
    printf("\n=== 測試基本檔案操作 ===\n");
    
    // 建立一個檔案
    int result = File_Create("test1.txt");
    test_result("建立檔案", result, 0);
    
    // 嘗試建立同名檔案，應該失敗
    result = File_Create("test1.txt");
    test_result("重複建立檔案", result, E_FILE_EXISTS);
    
    // 開啟檔案
    int fd = File_Open("test1.txt");
    test_result("開啟檔案", fd >= 0, 1);
    
    // 寫入檔案
    char buffer[100] = "Hello, TinyFS!";
    result = File_Write(fd, buffer, strlen(buffer) + 1);
    test_result("寫入檔案", result, strlen(buffer) + 1);
    
    // 關閉檔案
    result = File_Close(fd);
    test_result("關閉檔案", result, 0);
    
    // 重新開啟檔案
    fd = File_Open("test1.txt");
    test_result("重新開啟檔案", fd >= 0, 1);
    
    // 讀取檔案
    char read_buffer[100] = {0};
    result = File_Read(fd, read_buffer, 100);
    test_result("讀取檔案", result, strlen(buffer) + 1);
    
    // 檢查讀取的內容
    result = strcmp(buffer, read_buffer);
    test_result("檢查讀取內容", result, 0);
    
    // 關閉檔案
    result = File_Close(fd);
    test_result("再次關閉檔案", result, 0);
    
    // 刪除檔案
    result = File_Delete("test1.txt");
    test_result("刪除檔案", result, 0);
    
    // 嘗試開啟不存在的檔案
    fd = File_Open("test1.txt");
    test_result("開啟不存在的檔案", fd, E_NO_SUCH_FILE);
}

// 測試檔案指標和部分讀寫
void test_file_pointer() {
    printf("\n=== 測試檔案指標和部分讀寫 ===\n");
    
    // 建立一個檔案
    int result = File_Create("test2.txt");
    test_result("建立檔案", result, 0);
    
    // 開啟檔案
    int fd = File_Open("test2.txt");
    test_result("開啟檔案", fd >= 0, 1);
    
    // 寫入部分資料
    char buffer1[100] = "First part, ";
    result = File_Write(fd, buffer1, strlen(buffer1));
    test_result("寫入第一部分", result, strlen(buffer1));
    
    // 寫入第二部分
    char buffer2[100] = "second part.";
    result = File_Write(fd, buffer2, strlen(buffer2) + 1); // 加上結束符號
    test_result("寫入第二部分", result, strlen(buffer2) + 1);
    
    // 關閉檔案
    result = File_Close(fd);
    test_result("關閉檔案", result, 0);
    
    // 重新開啟檔案
    fd = File_Open("test2.txt");
    test_result("重新開啟檔案", fd >= 0, 1);
    
    // 僅讀取部分資料
    char read_buffer[100] = {0};
    result = File_Read(fd, read_buffer, 10);
    test_result("讀取部分資料", result, 10);
    read_buffer[10] = '\0';
    result = strcmp(read_buffer, "First part");
    test_result("檢查部分讀取內容", result, 0);
    
    // 讀取剩餘資料
    result = File_Read(fd, read_buffer, 100);
    test_result("讀取剩餘資料", result, strlen(", second part.") + 1);
    result = strcmp(read_buffer, ", second part.");
    test_result("檢查剩餘讀取內容", result, 0);
    
    // 關閉檔案
    result = File_Close(fd);
    test_result("關閉檔案", result, 0);
    
    // 刪除檔案
    result = File_Delete("test2.txt");
    test_result("刪除檔案", result, 0);
}

// 測試檔案大小限制
void test_file_size_limit() {
    printf("\n=== 測試檔案大小限制 ===\n");
    
    // 建立一個檔案
    int result = File_Create("large.txt");
    test_result("建立檔案", result, 0);
    
    // 開啟檔案
    int fd = File_Open("large.txt");
    test_result("開啟檔案", fd >= 0, 1);
    
    // 建立較大的資料
    char large_buffer[BLOCK_SIZE * 6];
    memset(large_buffer, 'A', BLOCK_SIZE * 6);
    
    // 嘗試寫入超過限制的資料
    result = File_Write(fd, large_buffer, BLOCK_SIZE * 6);
    test_result("寫入超過大小限制的資料", result, E_FILE_TOO_BIG);
    
    // 寫入正好符合大小限制的資料
    result = File_Write(fd, large_buffer, BLOCK_SIZE * 5);
    test_result("寫入最大允許大小的資料", result, BLOCK_SIZE * 5);
    
    // 關閉檔案
    result = File_Close(fd);
    test_result("關閉檔案", result, 0);
    
    // 刪除檔案
    result = File_Delete("large.txt");
    test_result("刪除檔案", result, 0);
}

// 測試同時開啟多個檔案
void test_multiple_open_files() {
    printf("\n=== 測試同時開啟多個檔案 ===\n");
    
    // 建立 6 個檔案
    for (int i = 0; i < 6; i++) {
        char filename[20];
        sprintf(filename, "file%d.txt", i);
        int result = File_Create(filename);
        test_result("建立檔案", result, 0);
    }
    
    // 開啟 5 個檔案 (最大允許數)
    int fd[MAX_OPEN_FILES + 1];
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        char filename[20];
        sprintf(filename, "file%d.txt", i);
        fd[i] = File_Open(filename);
        test_result("開啟檔案", fd[i] >= 0, 1);
    }
    
    // 嘗試開啟第 6 個檔案，應該失敗
    char filename[20];
    sprintf(filename, "file%d.txt", MAX_OPEN_FILES);
    fd[MAX_OPEN_FILES] = File_Open(filename);
    test_result("開啟超過限制的檔案", fd[MAX_OPEN_FILES], E_TOO_MANY_OPEN_FILES);
    
    // 關閉第一個檔案
    int result = File_Close(fd[0]);
    test_result("關閉檔案", result, 0);
    
    // 現在應該可以開啟第 6 個檔案
    fd[MAX_OPEN_FILES] = File_Open(filename);
    test_result("重新嘗試開啟檔案", fd[MAX_OPEN_FILES] >= 0, 1);
    
    // 關閉所有檔案
    for (int i = 1; i < MAX_OPEN_FILES; i++) {
        result = File_Close(fd[i]);
        test_result("關閉檔案", result, 0);
    }
    result = File_Close(fd[MAX_OPEN_FILES]);
    test_result("關閉檔案", result, 0);
    
    // 刪除所有檔案
    for (int i = 0; i < 6; i++) {
        char filename[20];
        sprintf(filename, "file%d.txt", i);
        result = File_Delete(filename);
        test_result("刪除檔案", result, 0);
    }
}

// 測試刪除已開啟的檔案
void test_delete_open_file() {
    printf("\n=== 測試刪除已開啟的檔案 ===\n");
    
    // 建立檔案
    int result = File_Create("openfile.txt");
    test_result("建立檔案", result, 0);
    
    // 開啟檔案
    int fd = File_Open("openfile.txt");
    test_result("開啟檔案", fd >= 0, 1);
    
    // 嘗試刪除已開啟的檔案
    result = File_Delete("openfile.txt");
    test_result("刪除已開啟的檔案", result, E_FILE_IN_USE);
    
    // 關閉檔案
    result = File_Close(fd);
    test_result("關閉檔案", result, 0);
    
    // 現在應該可以刪除檔案
    result = File_Delete("openfile.txt");
    test_result("刪除已關閉的檔案", result, 0);
}

// 測試無效的檔案描述符
void test_invalid_fd() {
    printf("\n=== 測試無效的檔案描述符 ===\n");
    
    // 使用無效的檔案描述符讀取
    int result = File_Read(-1, NULL, 10);
    test_result("使用負數描述符讀取", result, E_BAD_FD);
    
    result = File_Read(MAX_OPEN_FILES + 10, NULL, 10);
    test_result("使用過大描述符讀取", result, E_BAD_FD);
    
    // 使用無效的檔案描述符寫入
    result = File_Write(-1, NULL, 10);
    test_result("使用負數描述符寫入", result, E_BAD_FD);
    
    // 使用無效的檔案描述符關閉
    result = File_Close(-1);
    test_result("使用負數描述符關閉", result, E_BAD_FD);
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        usage(argv[0]);
    }
    char *path = argv[1];

    printf("啟動 TinyFS 檔案系統測試...\n");
    
    // 初始化檔案系統
    int result = FS_Boot(path);
    test_result("檔案系統初始化", result, 0);
    
    // 執行各種測試
    test_basic_operations();
    test_file_pointer();
    test_file_size_limit();
    test_multiple_open_files();
    test_delete_open_file();
    test_invalid_fd();
    
    printf("\n所有測試完成！\n");
    
    return 0;
}
