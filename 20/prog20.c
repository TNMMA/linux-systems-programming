#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  pid_t parent, child;
  int i;

  /* Parent process */
  parent=getpid();
  if( (child=fork()) == -1) {
    handle_error("fork");
  }

  if(child) {
    for(i=0;i<10000;i++) {
      printf("In Parent %d, waiting for child %d\n", parent, child);
    }
  } else {
    /* Child process */
    child=getpid();
    for(i=0;i<10000;i++) {
      printf("In Child %d, forked from parent %d\n", child, parent);
    }
  }
  return EXIT_SUCCESS;
}
