#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  int i;

  for(i=0; i<32; i++) {
    printf("%d.%s\n", i, strsignal(i));
  }
  return EXIT_SUCCESS;
}
