#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while(0)

int promptAndParse(char *cmd, char **upstream, char **downstream) {
  int i;
  int parseDownstream;
  /* Generate shell */
  printf("ashish> ");
  
  /* parse command provided */
  if(fgets(cmd, PATH_MAX, stdin) == NULL) {
    handle_error("fgets");
  }
  
  printf("Before tokenizing = %s\n", cmd);
  
  /* Extract \n from end of line */
  i=0;
  upstream[i] = strtok(cmd, " \t\n");
  printf("upstream[%d] = %s\n", i, upstream[i]);
  i++;
  while( (upstream[i] = strtok(NULL, " \t\n")) ) {
    if(strcmp(upstream[i], "|") == 0) {
      upstream[i] = NULL;
      parseDownstream = 1;
      i=0;
      break;
    }
    printf("upstream[%d] = %s\n", i, upstream[i]);
    i++;
  }
  downstream[0] = NULL;
  if(parseDownstream) {
    while( (downstream[i] = strtok(NULL, " \t\n")) ) {
      printf("downstream[%d] = %s\n", i, downstream[i]);
      i++;
    }
  }
  return 1;
}

int main(int argc, char *argv[]) {
  int p[2];
  int status1, status2;
  pid_t child1, child2;
  char cmd[PATH_MAX+1];
  char *upstream[PATH_MAX+1];
  char *downstream[PATH_MAX+1];

  while(promptAndParse(cmd, upstream, downstream)) {

    if(downstream[0] == NULL) {
      /* Only upstream exists, single child case */
      if( (child1 = fork()) == 0) {
        /* Child process */
        execvp(upstream[0], upstream);
        /* Dont come here unless child failed */
        printf("Unknown command %s\n", upstream[0]);
        exit(EXIT_FAILURE);
      } else {
        /* Parent process */
        /* Wait for child to complete */
        waitpid(child1, &status1, 0);
        if(WIFSIGNALED(status1)) {
          printf("Child exited due to signal = %d\n", WTERMSIG(status1));
#ifdef WCOREDUMP
          printf("Core Dump flag = %d\n", WCOREDUMP(status1));
#endif
        }
      }
    } else {
      /* downstream also contains a command, so need to fork twice */
      /* p[0] --> read end of the pipe
       * p[1] --> write end of the pipe
       */
      if(pipe(p) == -1) {
        handle_error("pipe creation");
      }
      if( (child1 = fork()) == 0) {
        /* upstream child process */
        /* Connect p[1] to stdout and close p[0] */
        if(dup2(p[1],1) == -1) {
          handle_error("dup2");
        }
        
        /* Close p[0] as its not relevant to child process */
        if(close(p[0]) == -1) {
          handle_error("close");
        }

        /* run command and output to p[1] */
        execvp(upstream[0], upstream); 
        /* Dont come here unless child failed */
        printf("Unknown command %s\n", upstream[0]);
        exit(EXIT_FAILURE);
      }
      if( (child2 = fork()) == 0) {
        /* downstream child process */
        /* Connect p[0] to stdin and close p[1] */
        if(dup2(p[0],0) == -1) {
          handle_error("dup2");
        }
        
        /* Close p[1] as its not relevant to child process */
        if(close(p[1]) == -1) {
          handle_error("close");
        }

        /* run command and output to p[1] */
        execvp(downstream[0], downstream); 
        /* Dont come here unless child failed */
        printf("Unknown command %s\n", downstream[0]);
        exit(EXIT_FAILURE);
      } else {
        /* Parent process */
        
        /* Close both ends of the pipe */
        close(p[0]);
        close(p[1]);
        /* Wait for child to complete */
        waitpid(child1, &status1, 0);
        waitpid(child2, &status2, 0);
        if(WIFSIGNALED(status1)) {
          printf("Child exited due to signal = %d\n", WTERMSIG(status1));
#ifdef WCOREDUMP
          printf("Core Dump flag = %d\n", WCOREDUMP(status1));
#endif
        }
        if(WIFSIGNALED(status2)) {
          printf("Child exited due to signal = %d\n", WTERMSIG(status2));
#ifdef WCOREDUMP
          printf("Core Dump flag = %d\n", WCOREDUMP(status2));
#endif
        }
      }
    }
  }
  return EXIT_SUCCESS;
}
