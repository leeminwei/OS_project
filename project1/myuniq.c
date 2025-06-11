#include <stdio.h>
#include <string.h>
#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
  //處理只有一個.txt
  if (argc == 2) {
    FILE *input;
    char filename[100];
    char buffer[MAX_BUFFER];
    sprintf(filename, "%s", argv[1]);
    input = fopen(filename, "r");

    //如果open失敗
    if (input == NULL) {
        printf("No such file or directory\n");
        return 0;
    }
    char prebuffer[MAX_BUFFER] = "";
    while (fgets(buffer, MAX_BUFFER, input) != NULL) {
        int repeat = 0;
        //如果相鄰的字串一樣
        if (strcmp(prebuffer, buffer) == 0) {
            repeat++;

        }
        //如果相鄰的字串不一樣
        else {
            printf("%s", buffer);
        }
        strcpy (prebuffer,buffer);
    }
  }

  //處理是否有-c的狀況
  else if (argc == 3){
    if (strcmp(argv[1],"-c")) {
        printf("input error\n");
    }
    else{
        int count = 0;
        FILE *input;
        char filename[100];
        char buffer[MAX_BUFFER];
        sprintf(filename, "%s", argv[2]);
        input = fopen(filename, "r");

        //如果open失敗
        if (input == NULL) {
            printf("No such file or directory\n");
            return 0;
        }
        char prebuffer[MAX_BUFFER] = "";
        while (fgets(buffer, MAX_BUFFER, input) != NULL) {
            // \n -> \0
            if (buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';
            if(count==0){
                strcpy (prebuffer,buffer);
                count++;
            }
            else if (strcmp(prebuffer,buffer) == 0) {
                count++;
            }
            else {
                printf("%d ", count);
                printf("%s\n", prebuffer);
                strcpy(prebuffer,buffer);
                count = 1;
            }
        }
        printf("%d ", count);
        printf("%s\n", prebuffer);
    }
  }
  //處理錯誤狀況
  else {
    printf("Too much file or directory\n");
  }
  return 0;
}