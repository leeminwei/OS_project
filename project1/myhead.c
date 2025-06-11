#include <stdio.h>
#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
    int count = 0;
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

    while (fgets(buffer, MAX_BUFFER, input) != NULL) {
        printf("%s", buffer);
        count++;
        if (count == 3) {
            break;
        }
    }
  }

  //處理錯誤狀況
  else {
    printf("Too much file or directory\n");
    return 0;
  }
  return 0;
}