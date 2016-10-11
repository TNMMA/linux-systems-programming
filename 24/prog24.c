#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  int p[2];
  pid_t child;
  /* p[0] --> read end of the pipe
   * p[1] --> write end of the pipe
   */
  if(pipe(p) == -1) {
    handle_error("pipe creation");
  }

  /* Spawn Child to execlp the input command */
  if( (child=fork()) == -1) {
    handle_error("fork");
  }
  
  if(child) {
    /* Parent process */
    /* Connect p[0] to stdin and close p[1] */
    if(dup2(p[0],0) == -1) {
      handle_error("dup2");
    }
    
    /* Close p[1] as its not relevant to parent process */
    if(close(p[1]) == -1) {
      handle_error("close");
    }

    /* run sort -n on whatever comes into stdin */
    execlp("sort", "sort", "-r", NULL); 
  } else {
    /* Connect p[1] to stout and close p[0] */
    if(dup2(p[1],1) == -1) {
      handle_error("dup2");
    }
    
    /* Close p[0] as its not relevant to child process */
    if(close(p[0]) == -1) {
      handle_error("close");
    }

    /* run sort -n on whatever comes into stdin */
    execlp("ls", "ls", NULL); 
  }
  
  return EXIT_SUCCESS;
}
