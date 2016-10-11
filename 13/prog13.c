#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)


int main(int argc, char *argv[]) {
  int i;
  for(i=1; i<argc; i++) {
    printf("%s ", argv[i]);
  }
  printf("\n");
  return EXIT_SUCCESS;
}
