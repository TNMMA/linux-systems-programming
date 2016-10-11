#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  pid_t parent, child;
  
  printf("EXIT_SUCCESS=%d\n,EXIT_FAILURE=%d\n", EXIT_SUCCESS, EXIT_FAILURE);
  if( (child=fork()) == -1) {
    handle_error("fork");
  }

  if(child) {
    /* Parent process */
    parent=getpid();
    printf("In Parent %d\n", parent);
  } else {
    /* Child process */
    child=getpid();
    printf("In Child %d\n", child);
  }
  return EXIT_SUCCESS;
}
