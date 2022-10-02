

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READING 0
#define WRITING 1

#define STDIN 0
#define STDOUT 1
#define STDERR 2


int
main(void)
{
  int ppipe[2];
  int cpipe[2];
  char buf[10];

  pipe(ppipe);
  pipe(cpipe);

  int pid = fork();
  if(pid < 0){
    fprintf(STDERR, "fork error\n");
    exit(1);
  } else if (pid == 0) {
    read(ppipe[READING], buf, 4);
    fprintf(STDOUT, "%d: received %s\n", getpid(), buf);
    write(cpipe[WRITING], "pong", 4);
  } else {
    write(ppipe[WRITING], "ping", 4);
    read(cpipe[READING], buf, 4);
    fprintf(STDOUT, "%d: received %s\n", getpid(), buf);
  }
  exit(0);
}
