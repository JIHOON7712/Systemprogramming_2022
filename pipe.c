#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 512

int main(void) {

    int fd1[2], fd2[2];
    int buffer = 0;
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fd1);
    pipe(fd2);

    if((pid = fork()) == 0){
        close(fd1[1]);
        close(fd2[0]);

        while(1){
            buffer = read(fd1[0], buf, BUF_SIZE);
            if(buffer == -1) exit(EXIT_FAILURE);
            if(buffer == 0) break;
            for(int i = 0; i < buffer; i++){
                if(buf[i] >= 'a' && buf[i] <= 'z') buf[i] -= 'a' - 'A';
                if(buf[i] == '\n') buf[i] = '\0';
            }
            write(fd2[1], buf, buffer);
        }
    }
    else if(pid == -1){
        exit(EXIT_FAILURE);
    }
    else{
        close(fd1[0]);
        close(fd2[1]);
                
        while(1){
            buffer = read(STDIN_FILENO, buf, BUF_SIZE);
            if(buffer == -1) exit(EXIT_FAILURE);
            if(buffer == 0) break;
            write(fd1[1], buf, buffer);
            read(fd2[0], buf, buffer);
            printf("%s\n", buf);
        }
    }
    return 0;
}