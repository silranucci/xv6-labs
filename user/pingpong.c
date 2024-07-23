#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
  int p1[2];
  int p2[2];

  if (pipe(p1) == -1 || pipe(p2) == -1) {
    fprintf(2, "error while creating pipe\n");
    exit(1);
  }

  if (fork() == 0) {
    char buf[1];
    read(p2[0], buf, 1);
    printf("%d: received ping\n", getpid());

    write(p1[1], "x", 1);
    close(p1[1]);
  } else {
    char buf[1];
    write(p2[1], "x", 1);
    close(p2[1]);
    read(p1[0], buf, 1);
    printf("%d: received pong\n", getpid());
  }
  exit(0);
}
