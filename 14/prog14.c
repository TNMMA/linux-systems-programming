#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

extern char **environ;

int main(int argc, char *argv[]) {
  char **p;

  for(p=environ; *p != NULL; p++) {
    printf("%s\n", *p);
  }

  return EXIT_SUCCESS;
}
