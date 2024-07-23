#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

#include "kernel/fcntl.h"
#include "kernel/fs.h"

static char buf[512];

void find(char *path, char *pattern) {
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s \n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s \n", path);
    close(fd);
    return;
  }

  switch (st.type) {
  case T_DEVICE:
  case T_FILE:
    break;
  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      fprintf(2, "find: path %s too long\n", path);
      close(fd);
      exit(1);
    }
    strcpy(buf, path);
    char *p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0)
        continue;
      if (strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (strcmp(pattern, de.name) == 0)
        printf("%s\n", buf);
      find(buf, pattern);
    }
    break;
  default:
    fprintf(2, "find: stat type %d error\n", st.type);
    close(fd);
    exit(1);
  }
  close(fd);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(2, "Usage: find [PATH] PATTERN\n");
    exit(1);
  }

  if (argc == 2)
    find(".", argv[1]);
  else {
    char *path = argv[1];
    char *pattern = argv[2];
    find(path, pattern);
  }

  exit(0);
}
