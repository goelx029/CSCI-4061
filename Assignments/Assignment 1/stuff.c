#include  <stdio.h>
#include  <string.h>
#include  <sys/types.h>

#define   MAX_COUNT  200
#define   BUF_SIZE   100

void dostuff() {
  pid_t  pid;
  pid = fork();
  if (pid) {
    wait(NULL);
    printf("In parent - pid of parent = %d\n", getpid());
    printf("In parent - pid of child = %d\n", pid);
  } else {
    printf("In child - pid of parent = %d\n", getppid());
    printf("In child - pid of child = %d\n", getpid());
    exit(0);
  }
}

void dostuff2() {
  pid_t pids[5];
  for (int i = 0; i < 5; i++) {
    pids[i] = fork();
    if (pids[i] == 0) {
      printf("%d\n", i);
      execl("ls", "ls", NULL);
      exit(0);
    } else {
      // wait();
    }
  }
  waitpid(-pids[4], NULL, 0);
  printf("%s\n", "\n\n\n\nLAUDA\n\n\n\n");
}

int factorial(int n) {
  if (n < 3) {
    return n;
  } else {
    int fds[2];
    pipe(fds);
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

void  main(void)
{
  printf("%d\n", factorial(3));
} 