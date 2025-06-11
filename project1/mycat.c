#include <stdio.h>
#define MAX_BUFFER 1024

int main(int argc, char *argv[]) {
  printf("We have %d arguments:\n", argc);

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
    }
  }

  //處理有兩個.txt
  else if (argc == 3){
    for (int i = 1; i < argc; ++i) {
        FILE *input;
        char filename[100];
        char buffer[MAX_BUFFER];
        sprintf(filename, "%s", argv[i]);
        input = fopen(filename, "r");

        //如果open失敗
        if (input == NULL) {
            printf("No such file or directory\n");
            return 0;
        }
        while (fgets(buffer, MAX_BUFFER, input) != NULL) {
            printf("%s", buffer);
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