#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
  int i;
  int tot_time = 0;

  if (argc < 2) {
    fprintf(2, "Usage: sleep TIME\n");
    exit(1);
  }

  for (i = 1; i < argc; i++)
    tot_time += atoi(argv[i]) * 10;

  sleep(tot_time);

  exit(0);
}
