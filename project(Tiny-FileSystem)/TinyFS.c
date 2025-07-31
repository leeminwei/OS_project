#include "TinyFS.h"
#include "TinyDisk.h"

#define MAX 1024
// TODO
// Define any constants you need here

// Define a structure for inode which contains filename, size and an integer array of size 5 that contains the inode indices [act as direct pointers]
typedef struct Inode{
    int valid;          // 是否被使用
    char filename[MAX]; // 檔案名稱
    int size;           // 檔案總大小
    int data_region[5]; // 一個inode最多可以記錄 5 * 512 bytes
}Inode;

// Define a structure for openfile which contains inode index, and the file pointer [after read write, file pointer might change]
typedef struct Openfile{
    int used;
    int inode_block;
    int file_pointer;
}Openfile;

// Now create inode table: an array: each member is an inode structure you just defined [figure out what would be the size of this]

Inode inode_table[MAX_FILES];

// Now create openfile table: an array: each member is an openfile structure you just defined [figure out what would be the size of this]

Openfile openfile_table[MAX_OPEN_FILES];

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for access()

#define MAGIC_NUMBER ((int)0xABCD1234)

// 假設的磁碟區塊分配（依照專案邏輯）：
#define SUPERBLOCK_INDEX 0
#define INODE_START_INDEX 1
#define INODE_BLOCKS 100
#define DATA_START_INDEX (INODE_START_INDEX + INODE_BLOCKS)

extern Block disk[NUM_BLOCKS]; // 假設你有這個定義

int FS_Boot(char *path) {
    printf("[FS_Boot] Path = %s\n", path);

    // 初始化記憶體磁碟
    if (Disk_Init() == -1) {
        printf("Disk_Init failed.\n");
        return E_DISK_ERROR;
    }

    // 嘗試讀入磁碟檔案（如果存在）
    if (access(path, F_OK) == 0) {
        printf("[FS_Boot] Disk file exists. Loading...\n");

        if (Disk_Load(path) == -1) {
            printf("Disk_Load failed.\n");
            return E_DISK_ERROR;
        }

        // 讀取 superblock 來驗證 magic number
        Block temp;
        if (Disk_Read(SUPERBLOCK_INDEX, (char*)&temp) == -1) {
            return E_DISK_ERROR;
        }

        int *magic_ptr = (int*)temp.data;
        if (*magic_ptr != MAGIC_NUMBER) {
            printf("Invalid magic number. Disk corrupted.\n");
            return E_DISK_ERROR;
        }

        printf("[FS_Boot] Existing disk loaded successfully.\n");
    } else {
        printf("[FS_Boot] No disk file found. Initializing new disk.\n");

        // 初始化 superblock
        Block super;
        memset(&super, 0, sizeof(Block));
        int *magic_ptr = (int*)super.data;
        *magic_ptr = MAGIC_NUMBER;
        if (Disk_Write(SUPERBLOCK_INDEX, (char*)&super) == -1) {
            return E_DISK_ERROR;
        }

        // 初始化 inode 區塊
        Block inode_block;
        memset(&inode_block, 0, sizeof(Block));
        for (int i = 0; i < INODE_BLOCKS; i++) {
            if (Disk_Write(INODE_START_INDEX + i, (char*)&inode_block) == -1) {
                return E_DISK_ERROR;
            }
        }

        // 初始化資料區塊（可以選擇不清除，如果預設為 0 就好）
        Block data_block;
        memset(&data_block, 0, sizeof(Block));
        for (int i = DATA_START_INDEX; i < NUM_BLOCKS; i++) {
            if (Disk_Write(i, (char*)&data_block) == -1) {
                return E_DISK_ERROR;
            }
        }

        // 儲存初始磁碟內容
        if (Disk_Save(path) == -1) {
            return E_DISK_ERROR;
        }

        printf("[FS_Boot] New disk initialized and saved.\n");
    }

    // 初始化開啟檔案表
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        openfile_table[i].used = 0;
        openfile_table[i].file_pointer = 0;
        openfile_table[i].inode_block = -1;
    }

    return 0;
}


int File_Create(char *file){
    // 遍歷每個Inode 比對檔案是否存在
    for (int i = 0; i < MAX_FILES; i++) {
        if (inode_table[i].valid == 1 &&
            strcmp(inode_table[i].filename, file) == 0) {
            return E_FILE_EXISTS;
        }
    }

    // 若檔名未存在，遍歷inode看是否有空間可以建立檔案
    for (int i = 0; i < MAX_FILES; i++) {
        if (inode_table[i].valid == 0) {
            inode_table[i].valid = 1;
            strcpy(inode_table[i].filename, file);
            inode_table[i].size = 0;
            memset(inode_table[i].data_region, 0, sizeof(inode_table[i].data_region));
            return 0;
        }
    }
    //如果沒有inode空間了
    return E_NO_SPACE;
}

int File_Open(char *file){
    // TODO
    int fd = -1;
    int file_exist = 0;
    int inode_idx = -1;
    //確認檔案是否存在
    for (int i=0 ; i<MAX_FILES ; i++) {
        if (inode_table[i].valid == 1 && strcmp(inode_table[i].filename, file) == 0) {
            file_exist = 1;
            inode_idx = i;
            break;
        }
    }
    if (file_exist == 0) {
        return E_NO_SUCH_FILE;
    }
    for (fd=0 ; fd<MAX_OPEN_FILES ; fd++) {
        if (openfile_table[fd].used == 0) {
            openfile_table[fd].used = 1;
            openfile_table[fd].file_pointer = 0;
            openfile_table[fd].inode_block = inode_idx;
            return fd;
        }
    }
    return E_TOO_MANY_OPEN_FILES;
}

int File_Read(int fd, void *buffer, int size){
    // TODO
    if (fd < 0 || fd >= MAX_OPEN_FILES || openfile_table[fd].used == 0) {
        return E_BAD_FD;
    }

    int inode_idx = openfile_table[fd].inode_block;
    int file_pointer = openfile_table[fd].file_pointer;
    int file_size = inode_table[inode_idx].size;

    // 如果(size + file_pointer當下的位置)大於檔案總大小
    // 那麼剩下要讀取的size = 總大小 - file_pointer
    if (file_pointer + size > file_size) {
        size = file_size - file_pointer;
    }

    //byte_read為目前讀取的byte數量
    int byte_read = 0;

    //用來存當前data_region的全部資料
    char temp[BLOCK_SIZE];

    while (byte_read < size) {
        int cur_pointer = file_pointer + byte_read;
        int cur_block = cur_pointer / BLOCK_SIZE;
        int offset = cur_pointer % BLOCK_SIZE;
        // 讀當前data_region的全部資料，並存到temp
        Disk_Read(inode_table[inode_idx].data_region[cur_block], temp);
        int to_copy = BLOCK_SIZE - offset;
        if (to_copy > size - byte_read) {
            to_copy = size - byte_read;
        }
        // 存在buffer + byte_read後面
        memcpy((char *)buffer + byte_read, temp + offset, to_copy);
        byte_read += to_copy;
    }
    // 更新 file pointer
    openfile_table[fd].file_pointer += byte_read;
    return byte_read;
}

int File_Write(int fd, void *buffer, int size){
    // TODO
    if (fd < 0 || fd >= MAX_OPEN_FILES || openfile_table[fd].used == 0) {
        return E_BAD_FD;
    }
    if (size > MAX_FILE_SIZE) {
        return E_FILE_TOO_BIG;
    }
    int inode_idx = openfile_table[fd].inode_block;
    int file_pointer = openfile_table[fd].file_pointer;

    if (file_pointer + size > MAX_FILE_SIZE) {
        return E_NO_SPACE;
    }

    int byte_write = 0;
    char temp[BLOCK_SIZE];
    while (byte_write < size) {
        int cur_pointer = file_pointer + byte_write;
        int cur_block = cur_pointer / BLOCK_SIZE;
        int offset = cur_pointer % BLOCK_SIZE;
        int to_write = BLOCK_SIZE - offset;
        // 若未配置，應先找空區塊配置
        if (Disk_Read(inode_table[inode_idx].data_region[cur_block], temp) == E_DISK_ERROR) {
            return E_DISK_ERROR;
        }
        if (size - byte_write < to_write) {
            to_write = size - byte_write;
        }
        memcpy(temp + offset, (char*)buffer + byte_write, to_write);
        Disk_Write(inode_table[inode_idx].data_region[cur_block], temp);
        byte_write += to_write;
    }
    openfile_table[fd].file_pointer += byte_write;

    if (openfile_table[fd].file_pointer > inode_table[inode_idx].size) {
        inode_table[inode_idx].size = openfile_table[fd].file_pointer;
    }

    return byte_write;
}

int File_Close(int fd)
{
    // TODO
    if (fd < 0 || fd >= MAX_OPEN_FILES || openfile_table[fd].used == 0) {
        return E_BAD_FD;
    }
    openfile_table[fd].used = 0;
    openfile_table[fd].file_pointer = 0;
    openfile_table[fd].inode_block = -1;
    return 0;
}

int File_Delete(char *file)
{
    // TODO
    for (int i=0 ; i<MAX_FILES ; i++) {
        if (inode_table[i].valid == 1 && strcmp(inode_table[i].filename, file) == 0) {
            for (int j=0 ; j<MAX_OPEN_FILES ; j++) {
                if (openfile_table[j].used == 1 && openfile_table[j].inode_block == i) {
                    return E_FILE_IN_USE;
                }
            }
            inode_table[i].valid = 0;
            strcpy(inode_table[i].filename, "");
            inode_table[i].size = 0;
            memset(inode_table[i].data_region, 0, sizeof(inode_table[i].data_region));
            return 0;
        }
    } 
    return E_NO_SUCH_FILE;
}


// Only graduate students implements the following
/* int FS_Sync(){
    // TODO
    return 0;
}

int File_Seek(int fd, int offset){
    // TODO
    return 0;
}
*/