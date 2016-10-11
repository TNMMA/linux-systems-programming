#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  struct stat fileInfo;

  if(argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
    exit(EXIT_SUCCESS);
  }

  if(stat(argv[1], &fileInfo) == -1) {
    handle_error("stat");
  }

  printf("last accessed: %s", ctime(&fileInfo.st_atime));
  printf("last modified: %s", ctime(&fileInfo.st_mtime));
  printf("last changed: %s", ctime(&fileInfo.st_ctime));
  return EXIT_SUCCESS;
}
