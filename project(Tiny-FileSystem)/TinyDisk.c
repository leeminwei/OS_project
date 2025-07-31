/*
 * DO NOT MODIFY THIS FILE
 */

#include "TinyDisk.h"

// the disk in memory
Block* disk;


/*
 * Initializes the disk area.
 * THIS FUNCTION MUST BE CALLED BEFORE ANY OTHER FUNCTION IN HERE CAN BE USED!
 */
int Disk_Init(){
    // create the disk image and fill every block with zeroes
    disk = (Block *)calloc(NUM_BLOCKS, sizeof(Block));
    if(disk == NULL) {
	    return E_DISK_ERROR;
    }
    return 0;
}

/*
 * Makes sure the current disk image gets saved to memory - this
 * will overwrite an existing file with the same name so be careful
 */
int Disk_Save(char* file) {
    FILE* diskFile;
    if (file == NULL) {
	    return E_DISK_ERROR;
    }
    
    // open the diskFile
    if ((diskFile = fopen(file, "w")) == NULL) {
	    return E_DISK_ERROR;
    }
    
    // actually write the disk image to a file
    // 寫入 NUM_BLOCKS 個 Block 到 diskFile
    if ((fwrite(disk, sizeof(Block), NUM_BLOCKS, diskFile)) != NUM_BLOCKS) {
	    fclose(diskFile);
	    return E_DISK_ERROR;
    }

    fclose(diskFile);
    return 0;
}

/*
 * Loads a current disk image from disk into memory - requires that
 * the disk be created first.
 */
int Disk_Load(char* file) {
    FILE* diskFile;
    if (file == NULL) {
	    return E_DISK_ERROR;
    }
    
    // open the diskFile
    if ((diskFile = fopen(file, "r")) == NULL) {
	    return E_DISK_ERROR;
    }
    
    // actually read the disk image into memory
    // 讀取 NUM_BLOCKS 個 Block，填入記憶體中的 disk 陣列
    if ((fread(disk, sizeof(Block), NUM_BLOCKS, diskFile)) != NUM_BLOCKS) {
	    fclose(diskFile);
	    return E_DISK_ERROR;
    }
    fclose(diskFile);
    return 0;
}

/*
 * Reads a single block from "disk" and puts it into a buffer provided
 * by the user.
 */
int Disk_Read(int block, char* buffer) {
    if ((block < 0) || (block >= NUM_BLOCKS) || (buffer == NULL)) {
	    return E_DISK_ERROR;
    }
    
    // 把「第 block 個區塊」的資料從 disk 複製到 buffer
    if((memcpy((void*)buffer, (void*)(disk + block), sizeof(Block))) == NULL) {
	    return E_DISK_ERROR;
    }
    
    return 0;
}

/*
 * Writes a single block from memory to "disk".
 */
int Disk_Write(int block, char* buffer) {
    if((block < 0) || (block >= NUM_BLOCKS) || (buffer == NULL)) {
	    return E_DISK_ERROR;
    }
    
    // 把資料從buffer複製到disk
    if((memcpy((void*)(disk + block), (void*)buffer, sizeof(Block))) == NULL) {
	    return E_DISK_ERROR;
    }
    return 0;
}
