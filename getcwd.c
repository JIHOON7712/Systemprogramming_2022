#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include<string.h>
#include<dirent.h>

void creatEnv();

/**
 * 현재 디렉터리의 절대경로를 반환한다.
 * @param buf   경로명을 저장할 버퍼 변수.
 * @param size  버퍼 변수의 크기
 * @return 버퍼 변수의 시작 주소, 즉 buf
 */


char *mygetcwd(char *buf, size_t size) {
    DIR* dirp;
    struct dirent* dp;
    struct stat current;
    struct stat parent_stat;

    char dirname[size];
    char buffer[size];

    if(lstat("..",&parent_stat))
        perror("Parent Stat Error");
    if(lstat(".",&current))
        perror("Current stat Error");
    if((dirp = opendir("..")) == NULL)
        perror("opendir Error");
    strcpy(buf,"\0");
    strcpy(dirname,"\0");

    while(current.st_ino != parent_stat.st_ino){
        while((dp = readdir(dirp)) != NULL){
            if(dp->d_ino == current.st_ino){
                strcpy(buffer,"/");
                strcat(buffer,dp->d_name);
                strcat(buffer,buf);
                strcpy(buf,buffer);
                break;
            }
        }
        strcat(dirname, "../");
        strcpy(buffer, dirname);
        if (lstat(buffer, &current))
            perror("Current stat Error!");
        strcpy(buffer, dirname);
        strcat(buffer, "..");
        if (lstat(buffer, &parent_stat))
            perror("Parent Stat Error");
        dirp = opendir(buffer);
    }
    closedir(dirp);
    return buf;
}

int main(void) {
  pid_t pid;
  int status;
  char buf[255];

  creatEnv();
  chdir("dir/sub");

  printf("original func: %s\n", getcwd(NULL, 0));
  printf("mygetcwd func: %s\n", mygetcwd(buf, 255));

  return 0;
}

void creatEnv(){
  mkdir("dir", 0755);
  mkdir("dir/sub", 0755);
  mkdir("dir/sub2", 0);
  
  creat("dir/a", 0755);
  creat("dir/b", 0755);
  creat("dir/sub/x", 0755);
  symlink("dir/a", "dir/sl");
  symlink("dir/x", "dir/dsl");
}