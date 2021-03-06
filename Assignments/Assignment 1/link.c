#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

int factorial(int n) {
  if (n < 3) {
    return n;
  } else {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid) {
      close(fd[1]);
      wait(NULL);
      char *buf = malloc(1024);
      read(fd[0], buf, 1024);
      return atoi(buf);
    } else {
      close(fd[0]);
      char *buf = malloc(1024);
      sprintf(buf, "%d", n*factorial(n-1));
      write(fd[1], buf, 1024);
      exit(0);
    }
  }
}

void foo(char* bar) {
     struct stat stats;

     lstat(bar, &stats);

     if (S_ISLNK(stats.st_mode)) {

          char s[1024];

          int size = readlink(bar, s, 1024);

          s[size] = '\0';

          unlink(bar);

          //link(s, bar);

     } else {
       unlink(bar);
     }

} 

int  main(int argc, char *argv[])
{
  symlink("AS", "A");
  link("A", "AH");
} 