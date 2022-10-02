
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READING 0
#define WRITING 1

#define STDIN 0
#define STDOUT 1
#define STDERR 2


void sieve(int, int);

int main (void) {
  int p[2];
  pipe(p);
  int pid = fork();
  if(pid == 0){
    close(p[WRITING]);
    sieve(p[READING], 2);
  } else {
    close(p[READING]);
    for (int i = 2; i < 35; i++) {
      write(p[WRITING], &i, sizeof(int));
    }
    close(p[WRITING]);
  }
  wait((int*) 0);
  exit(0);
}

void sieve(int pread, int prime){
  printf("prime %d\n", prime);
  char has_child = 0;
  int n;
  int p[2];
  while (read(pread, &n, sizeof(int)) != 0) {
    if(n%prime != 0){
      if(!has_child){
        has_child = 1;
        pipe(p);
        int pid = fork();
        if(pid < 0){
          exit(0);
        } else if(pid==0){
          close(p[WRITING]);
          sieve(p[READING], n);
        } else {
          close(p[READING]);
        }
      } else {
        write(p[WRITING], &n, sizeof(int));
      }
    }
  }
  close(p[WRITING]);
  wait((int*) 0);
  exit(0);
}
