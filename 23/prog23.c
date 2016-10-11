#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  pid_t child;
  int status;
  char cmd[PATH_MAX+1];

  while(1) {
    /* Generate shell */
    printf("ashish> ");
    
    /* parse command provided */
    if(fgets(cmd, PATH_MAX, stdin) == NULL) {
      handle_error("fgets");
    }

    /* Spawn Child to execlp the input command */
    if( (child=fork()) == -1) {
      handle_error("fork");
    }

    if(child) {
      /* Parent process */
      /* Wait for child to complete */
      waitpid(child, &status, 0);
      if(WIFSIGNALED(status)) {
        printf("Child exited due to signal = %d\n", WTERMSIG(status));
#ifdef WCOREDUMP
        printf("Core Dump flag = %d\n", WCOREDUMP(status));
#endif
      }
    } else {
      /* Child process */
      child=getpid();
      strtok(cmd, "\n");
      execlp(cmd, cmd, NULL);
      
      /* Dont come here unless child failed */
      printf("Unknown command %s\n", cmd);
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
