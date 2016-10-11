#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(int argc, char *argv[]) {
  pid_t parent, child;
  int status;
  int i;
  char *cmd[] = {"ps", "ls", "date", "goof"};

  parent=getpid();
  if( (child=fork()) == -1) {
    handle_error("fork");
  }

  if(child) {
    /* Parent process */
    printf("Parent with pid = %d\nJust sapwned Child with pid = %d\n", parent, child);
    /* Wait for child to complete */
    waitpid(child, &status, 0);
    printf("child exited with status = 0x%08x\n", status);
    if(WIFEXITED(status)) {
      printf("Child exited with status = %d\n", WEXITSTATUS(status));
    }

    if(WIFSIGNALED(status)) {
      printf("Child exited due to signal = %d\n", WTERMSIG(status));
      #ifdef WCOREDUMP
      printf("Core Dump flag = %d\n", WCOREDUMP(status));
      #endif
    }
  } else {
    /* Child process */
    child=getpid();
    printf("Running child with pid = %d\n", child);
    /* Generate menu */
    printf("1. ps\t2. ls\t3. date\t4. goof\n");
    if(scanf("%d", &i)==EOF) {
      handle_error("scanf");
    }
    if(i > 0 && i < 5) {
      printf("Found command %s\n", cmd[i-1]); 
      execlp(cmd[i-1], cmd[i-1], NULL);
    } else {
      printf("Unknown command option %d\n", i);
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
