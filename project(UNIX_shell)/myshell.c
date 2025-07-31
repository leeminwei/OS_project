#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

    while(1){
        printf("myshell> ");
        char input[1024];
        //scanf("%s", input);
        fgets(input, 100, stdin);

        //fgets會讀到'\n',所以要換成\0
        int len = strlen(input);
        if(input[len-1]=='\n') input[len-1] = '\0';
        printf("the input is : %s\n", input);

        //如果使用者輸入是exit
        if (strcmp(input,"exit") == 0)
        exit(0);

        //分割使用者輸入的字串
        const char* d = " ";
        char* user_command[100];
        int index = 0;
        char *p;
        p = strtok(input, d);
        while (p != NULL) {
            printf("%s\n", p);
            user_command[index++] = p;
            p = strtok(NULL, d);		   
        }
        user_command[index] = '\0';

        pid_t pid = fork();
        if(pid == 0){
            execvp(user_command[0],user_command);
            perror("execvp failed");
            exit(1);
        }
        else{
            waitpid(pid,NULL,0);
        }
    }
    return 0;
}